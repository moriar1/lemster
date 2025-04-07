#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    a.setOrganizationName("lemster");
    a.setApplicationName("lemster");
    MainWindow w;
    w.show();
    return a.exec();
}
