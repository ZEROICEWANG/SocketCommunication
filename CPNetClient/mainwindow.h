#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QThread>
#include <QObject>
#include "QPushButton"
#include "QLabel"
#include "QTimer"
#include "QListWidgetItem"
#include "QModelIndex"
#include "QLineEdit"
#include "addfriend.h"

namespace Ui {
class MainWindow;
}

//线程类
class portThread:public QThread
{
    Q_OBJECT
public:
    portThread();
    ~portThread();
    QString serverIP;
    QTcpSocket *mainSocket;
    int mainPort;
    int personalPort;
    void run();
signals:
    void perosonalPortisReady();

public slots:
    void getPersoanlPort();

};




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTcpSocket *personalSocket;
    QString serverIP;
    int personalPort;
    portThread* getportThread;
    AddFriend *add;
    QString username;
    QString account;
    QString password;
    QStringList friends;
    QString statue;
    bool login_statue;
    bool in_user_interface;
    bool is_logining;
    bool is_registing;

public:
    QListWidget *widget;
    QStringList list_of_dia;

private:
    QPushButton *backtoIni;
    //初始界面控件
    QLabel *label_acc;
    QLabel *label_username;
    QLabel *label_pass;
    QLineEdit *edi_acc;
    QLineEdit *edi_pass;
    QLineEdit *edi_username;
    QPushButton *btn_login;
    QPushButton *btn_register;
    QPushButton *btn_set;
    QLabel *label_statue;
        //注册界面控件
        QPushButton *btn_confirm;
        QLabel *label_confirmpassword;
        QLineEdit *edi_confirmpassword;
    //登录中界面
    QLabel *label_logining;
    QPushButton *btn_retry;
    QTimer *timer;
    int counter;



private:
    void setInitialInterfce();
private slots:
    void login();
    void registerTo();
    void setRegisterInterface();
    void setLoginInterface();
    void setUserInterface();
    void setBackToInitialInterface();
    void setLoginFailInterface();
    void timerOfDot();
private slots:
    void itemIsClicked(const QModelIndex &index);
    void addFriend();
private:
    void deleteIniControl();
private slots:
    void readyToConnect();
    void portdisconnect();
    void receiverMessage();
    void sendMessage(const QString mess);
    void refresh();
    void resetIP();
    void ipIsReady(QString ip);
signals:
    void recvMessage(QString mess);
    void getSearchresult();

private:
    void checkFile();

};

#endif // MAINWINDOW_H
