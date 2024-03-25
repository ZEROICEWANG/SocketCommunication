#ifndef DIALOGINTERFACE_H
#define DIALOGINTERFACE_H

#include <QDialog>
#include "QLabel"
#include "QPushButton"
#include "QTextEdit"
#include "QLineEdit"
#include "QStringList"
#include "QFile"
#include "mainwindow.h"
#include "QListWidget"
#include "QCloseEvent"

namespace Ui {
class DialogInterface;
}

class DialogInterface : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInterface(QString useraccout, QString account, QString name,QWidget *parent = 0);
    ~DialogInterface();

private:
    Ui::DialogInterface *ui;
    QFile *file;

private:
    QString message;
    QString record;
    QString historyRecord;
    QString Ip;
    QString mac;
    QString useraccount;
    QString friendname;
    QString friendaccount;
    QStringList list;

    QLineEdit *edi_uname;
    QLineEdit *edi_acc;
    QLineEdit *edi_Ip;
    QPushButton *btn_getIp;
    QTextEdit *edi_record;
    QTextEdit *edi_send;
    QPushButton *btn_sendMessage;

public:
    QString username;
    QListWidget *widget;
    QStringList *accountlist;
private slots:
    void getIp();
    void sendMessage();
    void showHistoryRecord();
    void deleteFrined();
public slots:
    void getMessage(QString mess);
signals:
    void messageIsReady(const QString mess);
private:
    void checkFile();

protected:
    void closeEvent(QCloseEvent* event);

};

#endif // DIALOGINTERFACE_H
