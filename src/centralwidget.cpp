#include "addpostdialog.h"
#include "centralwidget.h"
#include <qguiapplication.h>
#include <qsettings.h>

#include <QAction>
#include <QDebug>
#include <QDir>
#include <QErrorMessage>
#include <QFile>
#include <QGridLayout>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QMessageBox>
#include <QPushButton>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QStandardPaths>
#include <QTableView>
#include <QTextBrowser>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>

CentralWidget::~CentralWidget() {
    QSqlDatabase::removeDatabase("posts.db"); // Is it needed?
}

CentralWidget::CentralWidget(QWidget *parent)
    : QWidget(parent)
    , networkAccessManager(new QNetworkAccessManager(this)) {
    // --- Open existing or create new database ---
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    // Find where to store data
    QString dbPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (dbPath.isEmpty()) {
        QString err = "Failed determine app data location";
        QMessageBox::critical(this, "Error", err);
        qFatal() << err;
    }
    // Create path to data storage
    QDir dir(dbPath);
    if (!dir.mkpath(dir.absolutePath())) {
        QString err = "Failed to determine app data location";
        QMessageBox::critical(this, "Error", err);
        qFatal() << err;
    }
    // Create db if not exists
    db.setDatabaseName(dbPath + "/posts.db");
    if (!db.open()) {
        QErrorMessage erDialog;
        erDialog.showMessage("Failed to open database.");
        erDialog.exec();
        qFatal() << "Failed to open database.";
    }
    QSqlQuery{}.exec("CREATE TABLE IF NOT EXISTS "
                     "posts(number INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT, "
                     "community_id INTEGER, body TEXT, url TEXT);");
    // Read Settings
    QString cfgPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (cfgPath.isEmpty()) {
        QString err = "Failed determine app data location";
        QMessageBox::critical(this, "Error", err);
        qFatal() << err;
    }
    settings = new QSettings(cfgPath + ".ini", QSettings::IniFormat, this);

    // --- GUI config ---
    // Model
    model = new QSqlTableModel(this);
    model->setTable("posts");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit); // TODO: undo action
    model->select();

    // Model View
    tableView = new QTableView(this);
    tableView->setModel(model);
    tableView->show();
    tableView->setMinimumWidth(700);

    // Buttons
    addPostButton = new QPushButton("Add post", this);
    postButton = new QPushButton("Post", this);
    connect(addPostButton, &QPushButton::clicked, this, &CentralWidget::addPostClicked);
    connect(postButton, &QPushButton::clicked, this, &CentralWidget::postClicked);

    // Layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(addPostButton, 0, 0);
    mainLayout->addWidget(postButton, 0, 1);
    mainLayout->addWidget(tableView, 1, 0, 1, 2);
    setLayout(mainLayout);

    // Context Menu (right click to delete record)
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &CentralWidget::showContextMenu);
}

void CentralWidget::addPostClicked() {
    AddPostDialog dialog(this);
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
    model->database().transaction();

    // Create new record
    QSqlRecord record = model->record();
    record.remove(record.indexOf("number"));
    record.setValue("name", dialog.name());
    record.setValue("community_id", dialog.communityId());
    record.setValue("url", dialog.url());
    record.setValue("body", dialog.body());

    // Insert record
    if (model->insertRecord(-1, record)) {
        if (model->submitAll()) {
            model->database().commit();
        } else {
            model->database().rollback();
            // TODO: write separate function? to HANDLE ALL ERRORS like this and return
            QString err = "Database submit error: " + model->lastError().text();
            qDebug() << err;
            QMessageBox::critical(this, "Error", err);
        }
    } else {
        QString err = "Insert record in database error: " + model->lastError().text();
        qDebug() << err;
        QMessageBox::critical(this, "Error", err);
        model->database().rollback();
    }
}

void CentralWidget::postClicked() {
    // Get data from table
    QSqlRecord rec = model->record(0);
    QString url = rec.value("url").toString();

    if (url.isEmpty()) {
        qDebug() << "Warning: no url";
    }

    // Check settings existing
    QString jwt = "Bearer " + settings->value("jwt").toString();
    if (jwt == "Beared ") {
        QMessageBox::critical(
            this, "Error", "JWT is empty. Use configuration menu to specify login credentials."
        );
        return;
    }
    QString lemmyInstance = settings->value("lemmyinstance").toString();
    if (lemmyInstance.isEmpty()) {
        QMessageBox::critical(
            this, "Error", "Lemmy instance is empty. Use configuration menu to specify it."
        );
        return;
    }
    QString baseUrl = "https://" + lemmyInstance;

    connect(this, &CentralWidget::readyToPost, this, &CentralWidget::createPost);
    // Upload image on Lemmy if `url` is local path
    if (!url.startsWith("https://") && !url.isEmpty()) {
        qDebug() << "Uploading image" << url << "on " << lemmyInstance;
        UploadImage(url, jwt, baseUrl);
    } else {
        emit readyToPost(url);
    }
}

// Create new post on Lemmy instance
void CentralWidget::createPost(QString url) {
    QSqlRecord rec = model->record(0);
    QString body = rec.value("body").toString();
    QString name = rec.value("name").toString();
    int communityId = rec.value("community_id").toInt();
    QString lemmyInstance = settings->value("lemmyinstance").toString();
    QString baseUrl = "https://" + lemmyInstance;
    QString jwt = "Bearer " + settings->value("jwt").toString();

    // Create post on Lemmy
    QNetworkRequest request(QUrl(baseUrl + "/api/v3/post"));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("authorization", jwt.toUtf8());
    request.setRawHeader("content-type", "application/json");
    QJsonObject json{{"name", name}, {"community_id", communityId}, {"url", url}, {"body", body}};
    qDebug() << "Posting.\nCommunity ID:" << communityId << "\nPost name:" << name
             << "\nURL:" << url << "\nBody:" << body;

    QNetworkReply *reply = networkAccessManager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [reply]() {
        QMessageBox messageBox;
        if (reply->error() != QNetworkReply::NoError) {
            messageBox.setText("Error. " + reply->errorString());
        } else {
            messageBox.setText("Post created. Server reply: " + reply->readAll());
        }
        messageBox.exec();
        reply->deleteLater();
    });
}

void CentralWidget::showContextMenu(const QPoint &pos) {
    QMenu contextMenu("Context menu", this);
    QAction actionRemove("Remove record", this);
    connect(&actionRemove, &QAction::triggered, this, &CentralWidget::removeDataPoint);
    contextMenu.addAction(&actionRemove); // local link?
    contextMenu.exec(mapToGlobal(pos)); //exec(QCursor::pos()); exec(e->globalPosition().toPoint());
}

// TODO: add deleted/posted section, handle errors
void CentralWidget::removeDataPoint() {
    int index = tableView->selectionModel()->currentIndex().row();
    model->database().transaction();
    if (model->removeRow(index)) {
        if (model->submitAll()) {
            if (model->database().commit()) {
            }
        }
    }
}

QSettings *CentralWidget::getSettingsPtr() {
    return settings;
}

void CentralWidget::UploadImage(QString path, QString jwt, QString baseUrl) {
    QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
    QHttpPart imagePart;
    imagePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("image/jpeg"));
    imagePart.setHeader(
        QNetworkRequest::ContentDispositionHeader,
        QVariant("form-data; name=\"images[]\"; filename=\"anyname\"")
    );

    QFile *file = new QFile(path);
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open " << path;
        QMessageBox::critical(this, "Error", "Failed to open file `" + path + "`");
        emit readyToPost("");
    }

    imagePart.setBodyDevice(file);
    file->setParent(multiPart);
    multiPart->append(imagePart);

    QNetworkRequest request(QUrl(baseUrl + "/pictrs/image"));
    request.setRawHeader("authorization", jwt.toUtf8());
    QNetworkReply *reply = networkAccessManager->post(request, multiPart);

    multiPart->setParent(reply);

    connect(reply, &QNetworkReply::finished, this, [reply, baseUrl, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(
                this, "Error", "Failed upload image. Server reply: " + reply->errorString()
            );
            qDebug() << "Error." << reply->errorString()
                     << "Code: " << reply->error(); // TODO: add window
            reply->deleteLater();
            emit readyToPost("");
        } else {
            qDebug() << "Image Uploaded";
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject json = doc.object();
            QString relativePath = json["files"][0]["file"].toString(); // TODO: check if empty
            if (relativePath.isEmpty() || !relativePath.endsWith("webp")) {
                qDebug() << "Error. Resieved wrong path to image: " << relativePath;
                QMessageBox::critical(
                    this, "Error", "Resieved wrong path to image: `" + relativePath + "`"
                );
            }

            QString url = baseUrl + "/pictrs/image/" + relativePath;
            qDebug() << "Uploaded Image URL: " << url;
            reply->deleteLater();
            emit readyToPost(url);
        }
    });
}
