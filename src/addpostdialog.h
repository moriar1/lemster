#ifndef ADDPOSTDIALOG_H
#define ADDPOSTDIALOG_H

#include <QDialog>

class QLineEdit;
class QPlainTextEdit;
class QDialogButtonBox;

class AddPostDialog : public QDialog {
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(int communityId READ communityId WRITE setCommunityId)
    Q_PROPERTY(QString url READ url WRITE setUrl)
    Q_PROPERTY(QString body READ body WRITE setBody)

public:
    explicit AddPostDialog(QWidget *parent = nullptr);
    ~AddPostDialog();

    // Getters
    QString name() const;
    int communityId() const;
    QString url() const;
    QString body() const;

    // Setters
    void setName(const QString &name);
    void setCommunityId(const int &communityId);
    void setUrl(const QString &url);
    void setBody(const QString &body);

private:
    QLineEdit *postNameLine;
    QLineEdit *communityIdLine;
    QLineEdit *urlLine;
    QPlainTextEdit *bodyLine;
};

#endif
