#include "logindialog.h"
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , jwtLine(new QLineEdit(this))
    , lemmyInstanceLine(new QLineEdit(this)) {
    // Labels
    QLabel *jwtLabel = new QLabel("JWT", this);
    jwtLabel->setBuddy(jwtLine);
    QLabel *lemmyInstanceLabel = new QLabel("Instance", this);
    lemmyInstanceLabel->setBuddy(lemmyInstanceLine);
    QLabel *lemmyInstanceNoteLabel = new QLabel("Example: lemmy.world", this);

    // Buttons
    QDialogButtonBox *dialogButtons
        = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(dialogButtons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    // Layout
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->addWidget(jwtLabel, 0, 0);
    mainLayout->addWidget(jwtLine, 0, 1);
    mainLayout->addWidget(lemmyInstanceLabel, 1, 0);
    mainLayout->addWidget(lemmyInstanceLine, 1, 1);
    mainLayout->addWidget(lemmyInstanceNoteLabel, 2, 0, 1, 2);
    mainLayout->addWidget(dialogButtons, 3, 1);
    setLayout(mainLayout);
    setWindowTitle("Login");
}

LoginDialog::~LoginDialog() {}

//// Getters ////
Config LoginDialog::config() const {
    return Config{
        jwtLine->text(),
        lemmyInstanceLine->text(),
    };
}

//// Setters ////
void LoginDialog::setConfig(const Config &config) {
    jwtLine->setText(config.jwt);
    jwtLine->setText(config.lemmyInstance);
}
