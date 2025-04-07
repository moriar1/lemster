#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "logindialog.h"
#include <QMainWindow>
// #include "cen

class CentralWidget;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onConfigurationClicked();

signals:
    void configSignal(Config); //add `&` ?

private:
    CentralWidget *centralWidget;
};

#endif
