#include "addpostdialog.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>

#include <QCompleter>
#include <QStringList>

AddPostDialog::AddPostDialog(QWidget *parent)
    : QDialog(parent)
    , postNameLine(new QLineEdit(this))
    , communityIdLine(new QLineEdit(this))
    , urlLine(new QLineEdit(this))
    , bodyLine(new QPlainTextEdit(this)) {
    // Labels
    QLabel *postNameLabel = new QLabel("Name", this); // Must be members?
    QLabel *urlLabel = new QLabel("URL", this);
    QLabel *communityIdLabel = new QLabel("Com ID", this);
    QLabel *bodyLabel = new QLabel("Body", this);
    postNameLabel->setBuddy(postNameLine); // TODO: ?
    urlLabel->setBuddy(urlLine);
    communityIdLabel->setBuddy(communityIdLine);
    bodyLabel->setBuddy(bodyLine);

    // Buttons
    QDialogButtonBox *dialogButtons
        = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // TEMP
    QStringList wordList;
    wordList << "alpha" << "omega" << "omicron" << "zeta";

    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    communityIdLine->setCompleter(completer);
    // TEMP

    // Layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(postNameLabel, 0, 0);
    mainLayout->addWidget(postNameLine, 0, 1, 1, 3);
    mainLayout->addWidget(communityIdLabel, 1, 0);
    mainLayout->addWidget(communityIdLine, 1, 1);
    mainLayout->addWidget(urlLabel, 1, 2);
    mainLayout->addWidget(urlLine, 1, 3);
    mainLayout->addWidget(bodyLabel, 2, 0);
    mainLayout->addWidget(bodyLine, 2, 1, 1, 3);
    mainLayout->addWidget(dialogButtons, 5, 3);
    setLayout(mainLayout);
}

AddPostDialog::~AddPostDialog() {}

//// Getters ////
QString AddPostDialog::name() const {
    return postNameLine->text();
}
int AddPostDialog::communityId() const {
    return communityIdLine->text().toInt();
}
QString AddPostDialog::url() const {
    return urlLine->text();
}
QString AddPostDialog::body() const {
    return bodyLine->toPlainText();
}

//// Setters ////
void AddPostDialog::setName(const QString &name) {
    postNameLine->setText(name);
}
void AddPostDialog::setCommunityId(const int &communityId) {
    communityIdLine->setText(QString("%1").arg(communityId));
}
void AddPostDialog::setUrl(const QString &url) {
    urlLine->setText(url);
}
void AddPostDialog::setBody(const QString &body) {
    bodyLine->setPlainText(body);
}
