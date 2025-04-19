#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QDialogButtonBox;
class QNetworkAccessManager;
class QSettings;

class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

public slots:
    void receiveNam(QNetworkAccessManager *);
    void receiveSettings(QSettings *l_settings);

private slots:
    void onOkButtonClicked();

signals:
    void LoggedIn();

private:
    QSettings *settings;
    QLineEdit *usernameLine;
    QLineEdit *passwordLine;
    QLineEdit *lemmyInstanceLine;
    QNetworkAccessManager *networkAccessManager = nullptr;
};

#endif
