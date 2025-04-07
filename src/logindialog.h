#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QDialogButtonBox;

struct Config {
    QString jwt;
    QString lemmyInstance;
};
Q_DECLARE_METATYPE(Config)

class LoginDialog : public QDialog {
    Q_OBJECT
    Q_PROPERTY(Config config READ config WRITE setConfig)

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
    Config config() const;
    void setConfig(const Config &config);

private:
    QLineEdit *jwtLine;
    QLineEdit *lemmyInstanceLine;
};

#endif
