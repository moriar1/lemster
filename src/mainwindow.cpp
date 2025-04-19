#include "centralwidget.h"
#include "logindialog.h"
#include "mainwindow.h"
#include <qcoreapplication.h>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(new CentralWidget(this))
    , settings(nullptr) {
    setCentralWidget(centralWidget);

    QAction *configurationAction = new QAction("Configuration", this);
    connect(configurationAction, &QAction::triggered, this, &MainWindow::onConfigurationClicked);

    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->addAction(configurationAction);
    setMenuBar(menuBar);

    // send NetworkAccessManager and Settings to LoginDialog (TODO: BUG `emit` only when ConfigurationClicked)
    loginDialog = new LoginDialog;
    connect(centralWidget, &CentralWidget::sendNam, loginDialog, &LoginDialog::receiveNam);
    connect(centralWidget, &CentralWidget::sendSettings, loginDialog, &LoginDialog::receiveSettings);
    emit centralWidget->sendNam(centralWidget->networkAccessManager);
    emit centralWidget->sendSettings(centralWidget->settings);
}

void MainWindow::onConfigurationClicked() {
    loginDialog->exec();
    // loginDialog->deleteLater(); // If uncomment will be segmentation fault ufter second ConfigurationClicked (see TODO above)
}

MainWindow::~MainWindow() {}
