#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CentralWidget;
class QSettings;
class LoginDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConfigurationClicked();

private:
    CentralWidget *centralWidget;
    QSettings *settings;
    LoginDialog *loginDialog;
};

#endif
