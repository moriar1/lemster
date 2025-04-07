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
    , centralWidget(new CentralWidget(this)) {
    setCentralWidget(centralWidget);

    QAction *configurationAction = new QAction("Configuration", this);
    connect(configurationAction, &QAction::triggered, this, &MainWindow::onConfigurationClicked);
    connect(this, &MainWindow::configSignal, centralWidget, &CentralWidget::setConfigSlot);

    QMenuBar *menuBar = new QMenuBar(this);
    menuBar->addAction(configurationAction);
    setMenuBar(menuBar);
}

void MainWindow::onConfigurationClicked() {
    LoginDialog dialog;
    if (dialog.exec() == QDialog::Rejected) {
        return;
    }
    emit configSignal(dialog.config());

    QString cfgPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (cfgPath.isEmpty()) {
        QString err = "Failed determine app data location";
        QMessageBox::critical(this, "Error", err);
        qFatal() << err;
    }
    QSettings settings(cfgPath + ".ini", QSettings::IniFormat);
    settings.setValue("jwt", dialog.config().jwt);
    settings.setValue("lemmyinstance", dialog.config().lemmyInstance);
    // qDebug() << settings.value("jwt");
}
MainWindow::~MainWindow() {}
