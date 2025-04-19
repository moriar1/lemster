#include "logindialog.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>

LoginDialog::~LoginDialog() {}

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , settings(nullptr)
    , usernameLine(new QLineEdit(this))
    , passwordLine(new QLineEdit(this))
    , lemmyInstanceLine(new QLineEdit(this)) {
    // Labels
    QLabel *usernameLabel = new QLabel("Username", this);
    usernameLabel->setBuddy(usernameLine);
    QLabel *passwordLabel = new QLabel("Password", this);
    usernameLabel->setBuddy(passwordLine);
    QLabel *lemmyInstanceLabel = new QLabel("Instance", this);
    lemmyInstanceLabel->setBuddy(lemmyInstanceLine);
    QLabel *lemmyInstanceNoteLabel = new QLabel("Example: lemmy.world", this);

    // Buttons
    QDialogButtonBox *dialogButtons
        = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    // connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::accepted, this, &LoginDialog::onOkButtonClicked);
    connect(this, &LoginDialog::LoggedIn, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(usernameLabel, 0, 0);
    mainLayout->addWidget(usernameLine, 0, 1);
    mainLayout->addWidget(passwordLabel, 1, 0);
    mainLayout->addWidget(passwordLine, 1, 1);
    mainLayout->addWidget(lemmyInstanceLabel, 2, 0);
    mainLayout->addWidget(lemmyInstanceLine, 2, 1);
    mainLayout->addWidget(lemmyInstanceNoteLabel, 3, 1); // 3, 0, 1, 2);
    mainLayout->addWidget(dialogButtons, 4, 1);
    setLayout(mainLayout);
    setWindowTitle("Login");
}

void LoginDialog::receiveNam(QNetworkAccessManager *l_nam) {
    networkAccessManager = l_nam;
}

void LoginDialog::receiveSettings(QSettings *l_settings) {
    settings = l_settings;
    if (settings->value("lemmyinstance").isValid()) {
        lemmyInstanceLine->setText(settings->value("lemmyinstance").toString());
    }
}

// Get JWT using provided login and password
// TODO: add indication of process
void LoginDialog::onOkButtonClicked() {
    if (usernameLine->text().isEmpty() || passwordLine->text().isEmpty()
        || lemmyInstanceLine->text().isEmpty()) {
        QMessageBox::critical(this, "Error", "You need do fill every field.");
        return;
    }

    QNetworkRequest request(QUrl("https://" + lemmyInstanceLine->text() + "/api/v3/user/login"));
    request.setRawHeader("accept", "application/json");
    request.setRawHeader("content-type", "application/json");
    QJsonObject json{{"username_or_email", usernameLine->text()}, {"password", passwordLine->text()}};

    QNetworkReply *reply = networkAccessManager->post(request, QJsonDocument(json).toJson());
    connect(reply, &QNetworkReply::finished, this, [reply, this]() {
        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(this, "Error", "Failed to login. " + reply->errorString());
        } else {
            QJsonDocument json = QJsonDocument::fromJson(reply->readAll());
            qDebug() << json["jwt"];
            if (settings->value("jwt").isValid() && settings->value("lemmyinstance").isValid()) {
                // TODO: lemmyLogout();
            }
            settings->setValue("lemmyinstance", lemmyInstanceLine->text());
            settings->setValue("jwt", json["jwt"].toString());
            emit LoggedIn();
        }
        reply->deleteLater();
    });
}
