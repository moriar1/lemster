#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include "logindialog.h"
#include <QModelIndex>
#include <QPushButton>
#include <QTableView>
#include <QWidget>

class QSqlTableModel;
class QNetworkAccessManager;
class QSettings;

class CentralWidget : public QWidget {
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();
    QSettings *getSettingsPtr();

signals:
    void sendNam(QNetworkAccessManager *);
    void sendSettings(QSettings *);
    void readyToPost(QString);

private slots:
    void addPostClicked();
    void showContextMenu(const QPoint &);
    void postClicked();
    void removeDataPoint();
    void createPost(QString); // Create new post on Lemmy instance

private:
    QSqlTableModel *model;
    QTableView *tableView;
    QPushButton *addPostButton;
    QPushButton *postButton;

    void UploadImage(QString, QString, QString);

public:
    QSettings *settings; // for reading/saving jwt, lemmy instance and completer in file
    QNetworkAccessManager *networkAccessManager;
};

#endif
