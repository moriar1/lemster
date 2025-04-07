#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "logindialog.h"
#include <QModelIndex>
#include <QPushButton>
#include <QTableView>
#include <QWidget>

class QSqlTableModel;
class QNetworkAccessManager;

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();
private slots:
    void addPostClicked();
    void showContextMenu(const QPoint &);
    void postClicked();
    void removeDataPoint();

public slots:
    void setConfigSlot(Config);

private:
    QNetworkAccessManager *networkAccessManager;
    QSqlTableModel *model;
    QTableView *tableView;
    QPushButton *addPostButton;
    QPushButton *postButton;
    Config config;

    QString UploadImage(QString, QString, QString);
};

#endif
