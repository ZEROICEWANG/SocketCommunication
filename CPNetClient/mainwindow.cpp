#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QObject"
#include "QByteArray"
#include "QLayout"
#include "QDebug"
#include "QPixmap"
#include "QListWidget"
#include "QListWidgetItem"
#include "dialoginterface.h"
#include "QByteArray"
#include "QStringList"
#include "addfriend.h"
#include "QMessageBox"
#include "historyrecord.h"
#include "QDir"
#include "QPalette"
#include "QFont"
#include "setting.h"
#include "QNetworkInterface"

QString getHostMacAddress();
//搜索部分
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(QStringLiteral(":/new/pict1/logo")));
    this->setWindowTitle(tr("CC"));
    serverIP="127.0.0.1";
    this->statue="offline";
    is_logining=false;
    in_user_interface=false;
    is_registing=false;
    personalSocket=new QTcpSocket();
    connect(personalSocket,&QTcpSocket::readyRead,this,&MainWindow::receiverMessage);
    connect(personalSocket,&QTcpSocket::disconnected,this,&MainWindow::portdisconnect);
    getportThread=new portThread();
    getportThread->serverIP=serverIP;
    connect(getportThread,&portThread::perosonalPortisReady,this,&MainWindow::readyToConnect,Qt::QueuedConnection);
    getportThread->start();
    setFixedSize(QSize(600,400));
    setInitialInterfce();
    //setUserInterface();
}

void MainWindow::login()
{
    is_logining=true;
    if(!login_statue)
    {
        delete label_logining;
        delete btn_retry;
        delete backtoIni;


    }else
    {
        account=edi_acc->text();
        password=edi_pass->text();
    }
    setLoginInterface();


    QByteArray data;
    QString mess="login|account="+account+"'password="+password+"'mac="+getHostMacAddress();
    qDebug()<<"登录信息已发送:"+mess;
    data=mess.toUtf8();
    personalSocket->write(data);
    personalSocket->flush();
}

void MainWindow::registerTo()
{
    QString passwordCondirm;
    username=edi_username->text();
    password=edi_pass->text();
    passwordCondirm=edi_confirmpassword->text();
    if(username=="")
    {
        QMessageBox mes(this);
        mes.setText("用户名不可为空");
        mes.setWindowTitle(tr(""));
        mes.exec();
    }
    else{
        if(password.length()<6)
        {
            QMessageBox mes(this);
            mes.setText("密码不可少于六位");
            mes.setWindowTitle(tr(""));
            mes.exec();
        }else
        {
            if(password!=passwordCondirm)
            {
                QMessageBox mes(this);
                mes.setText("两次密码不一致");
                mes.setWindowTitle(tr(""));
                mes.exec();
            }
            else{
                QString mess="register|username="+username+"'password="+password;
                QByteArray data=mess.toUtf8();
                qDebug()<<"注册信息已发送："+mess;
                personalSocket->write(data);
                personalSocket->flush();
            }
        }
    }
}

void MainWindow::setInitialInterfce()
{
    login_statue=true;
    is_logining=false;
    is_registing=false;
    label_statue=new QLabel();
    this->layout()->addWidget(label_statue);
    label_statue->setGeometry(0,0,60,20);
    QFont font ("Microsoft YaHei", 8, 70); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    label_statue->setFont(font);
    label_statue->setText(statue);

    btn_set=new QPushButton();
    this->layout()->addWidget(btn_set);
    btn_set->setGeometry(550,0,50,28);
    QFont font_set ("Microsoft YaHei", 7, 50);
    btn_set->setFont(font_set);
    btn_set->setText("设置");

    label_acc=new QLabel();
    this->layout()->addWidget(label_acc);
    label_acc->setText("账号");
    label_acc->setGeometry(140,100,41,41);
    label_pass=new QLabel();
    this->layout()->addWidget(label_pass);
    label_pass->setText("密码");
    label_pass->setGeometry(140,180,41,41);
    edi_acc=new QLineEdit();
    this->layout()->addWidget(edi_acc);
    edi_acc->setGeometry(200,100,231,41);
    edi_pass=new QLineEdit();
    this->layout()->addWidget(edi_pass);
    edi_pass->setGeometry(200,180,231,41);
    edi_pass->setEchoMode(QLineEdit::Password);
    btn_login=new QPushButton();
    this->layout()->addWidget(btn_login);
    btn_login->setText("登录");
    btn_login->setDefault(true);
    btn_login->setGeometry(225,270,171,41);
    btn_register=new QPushButton();
    this->layout()->addWidget(btn_register);
    btn_register->setText("注册");
    btn_register->setGeometry(519,359,81,41);
    connect(btn_login,&QPushButton::clicked,this,&MainWindow::login);
    connect(btn_register,&QPushButton::clicked,this,&MainWindow::setRegisterInterface);
    connect(btn_set,&QPushButton::clicked,this,&MainWindow::resetIP);

    if(this->statue=="offline")
    {
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::red);
        label_statue->setPalette(pe);
        btn_login->setDisabled(true);
        btn_register->setDisabled(true);
    }else
    {
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::green);
        label_statue->setPalette(pe);
    }

    qDebug()<<"初始界面设置成功";
}

void MainWindow::setRegisterInterface()
{
    deleteIniControl();

    is_registing=true;
    label_username=new QLabel();
    this->layout()->addWidget(label_username);
    label_username->setText("昵称");
    label_username->setGeometry(120,80,41,41);
    label_pass=new QLabel();
    this->layout()->addWidget(label_pass);
    label_pass->setText("密码");
    label_pass->setGeometry(120,140,41,41);

    edi_username=new QLineEdit();
    this->layout()->addWidget(edi_username);
    edi_username->setGeometry(200,80,231,41);
    edi_pass=new QLineEdit();
    this->layout()->addWidget(edi_pass);
    edi_pass->setGeometry(200,140,231,41);

    label_confirmpassword=new QLabel();
    this->layout()->addWidget(label_confirmpassword);
    label_confirmpassword->setText("确认密码");
    label_confirmpassword->setGeometry(120,200,82,41);
    edi_confirmpassword =new QLineEdit();
    this->layout()->addWidget(edi_confirmpassword);
    edi_confirmpassword->setGeometry(200,200,231,41);


    btn_confirm=new QPushButton();
    this->layout()->addWidget(btn_confirm);
    btn_confirm->setText("注册");
    btn_confirm->setGeometry(230,270,171,41);
    backtoIni=new QPushButton();
    this->layout()->addWidget(backtoIni);
    backtoIni->setText("返回");
    backtoIni->setGeometry(519,359,81,41);
    connect(backtoIni,&QPushButton::clicked,this,&MainWindow::setBackToInitialInterface);
    connect(btn_confirm,&QPushButton::clicked,this,&MainWindow::registerTo);
}

void MainWindow::setLoginInterface()
{
    if(login_statue)
    {
        deleteIniControl();
    }
    timer=new QTimer();
    counter=0;
    label_logining=new QLabel();
    this->layout()->addWidget(label_logining);
    label_logining->setAlignment(Qt::AlignLeft);
    label_logining->setGeometry(220,180,150,40);
    connect(timer,&QTimer::timeout,this,&MainWindow::timerOfDot);
    timer->start(500);
    qDebug()<<"timer";
}

void MainWindow::setLoginFailInterface()
{
    login_statue=false;
    timer->stop();
    delete timer;
    delete label_logining;
    label_logining =new QLabel();
    this->layout()->addWidget(label_logining);
    label_logining->setGeometry(255,150,150,40);
    label_logining->setText("连接超时");
    btn_retry=new QPushButton();
    this->layout()->addWidget(btn_retry);
    btn_retry->setGeometry(215,250,171,41);
    btn_retry->setText("重试");
    backtoIni=new QPushButton();
    this->layout()->addWidget(backtoIni);
    backtoIni->setGeometry(519,359,81,41);
    backtoIni->setText("返回");
    connect(btn_retry,&QPushButton::clicked,this,&MainWindow::login);
    connect(backtoIni,&QPushButton::clicked,this,&MainWindow::setBackToInitialInterface);    
}

void MainWindow::setUserInterface()
{
    checkFile();
    is_logining=false;
    in_user_interface=true;
    setFixedSize(QSize(400,800));
    QLabel *label_icon=new QLabel();
    this->layout()->addWidget(label_icon);
    label_icon->setGeometry(0,0,150,150);
    QPixmap *pix=new QPixmap(":/new/pict1/icon");
    pix->scaled(label_icon->size(),Qt::KeepAspectRatio);
    label_icon->setScaledContents(true);
    label_icon->setPixmap(*pix);
    label_acc=new QLabel();
    label_username=new QLabel();
    label_statue=new QLabel();


    QFont font ("Microsoft YaHei", 8, 70); //第一个属性是字体（微软雅黑），第二个是大小，第三个是加粗（权重是75）
    label_statue->setFont(font);
    label_statue->setText(statue);
    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::green);
    label_statue->setPalette(pe);

    QPushButton *btn_AddFriend=new QPushButton();
    QPushButton *btn_refresh=new QPushButton();

    this->layout()->addWidget(label_acc);
    label_acc->setGeometry(155,0,185,30);
    this->layout()->addWidget(label_username);
    label_username->setGeometry(155,40,240,30);
    this->layout()->addWidget(label_statue);
    label_statue->setGeometry(340,0,60,30);
    this->layout()->addWidget(btn_AddFriend);
    btn_AddFriend->setGeometry(155,70,240,30);
    this->layout()->addWidget(btn_refresh);
    btn_refresh->setGeometry(155,110,240,30);
    label_acc->setText(this->account);
    label_username->setText(this->username);
    label_statue->setText(statue);
    btn_AddFriend->setText(tr("添加好友"));
    btn_refresh->setText(tr("刷新"));

    widget=new QListWidget(this);
    this->layout()->addWidget(widget);
    widget->setGeometry(0,151,400,649);
    QListWidgetItem *item;
    qDebug()<<"friend";
    for(int i=0;i<friends.length();i++)
    {
        item=new QListWidgetItem();
        item->setText(friends[i].split("=")[0]+"("+friends[i].split("=")[1]+")");
        widget->addItem(item);
    }

    connect(widget,&QListWidget::doubleClicked,this,&MainWindow::itemIsClicked);

    connect(btn_AddFriend,&QPushButton::clicked,this,&MainWindow::addFriend);
    connect(btn_refresh,&QPushButton::clicked,this,&MainWindow::refresh);
}

void MainWindow::checkFile()
{
    QDir *dir=new QDir("E:");
    if(!dir->exists("CC"))
    {
        dir->mkdir("CC");
    }
    dir->setCurrent("CC");
    dir->cd("CC");
    if(!dir->exists(account))
    {
        dir->mkdir(account);
    }
    dir->setCurrent(dir->currentPath()+account);
    dir->cd(dir->currentPath()+account);
}

void MainWindow::deleteIniControl()
{
    delete label_acc;
    delete label_pass;
    delete edi_acc;
    delete edi_pass;
    delete btn_login;
    delete btn_register;
    delete btn_set;
    delete label_statue;
}

void MainWindow::setBackToInitialInterface()
{
    if(login_statue)
    {
        delete label_username;
        delete label_pass;
        delete label_confirmpassword;
        delete edi_username;
        delete edi_pass;
        delete edi_confirmpassword;
        delete btn_confirm;
        delete backtoIni;
    }
    else
    {
        delete label_logining;
        delete btn_retry;
        delete backtoIni;
    }
    setInitialInterfce();
}

void MainWindow::receiverMessage()
{
    QByteArray data;
    QString message;
    QStringList list;
    data=personalSocket->readAll();
    if(!data.isEmpty())
    {
        QString message=tr(data);
        qDebug()<<"get a message: "+message;
        list=message.split("|");
        if(list[0]=="chat"||list[0]=="IP")
        {
            if(list[0]=="chat")
            {
                bool statue_=false;
                QString temp_account=list[1].split("'")[1].split("=")[1];
                for(int i=0;i<this->list_of_dia.length();i++)
                {
                   if (list_of_dia[i]==temp_account)
                   {
                       statue_=true;
                   }
                }
                if(!statue_)
                {
                    //chat|friendname=friendname’account=account’message=message
                    QString friendsname=list[1].split("'")[0].split("=")[1];
                    QString friendsaccount=list[1].split("'")[1].split("=")[1];
                    this->list_of_dia.append(friendsaccount);
                    DialogInterface *dia=new DialogInterface(account, friendsaccount, friendsname);
                    dia->username=username;
                    dia->widget=this->widget;
                    dia->accountlist=&list_of_dia;
                    dia->show();
                    connect(this,&MainWindow::recvMessage,dia,&DialogInterface::getMessage);
                    connect(dia,&DialogInterface::messageIsReady,this,&MainWindow::sendMessage);
                }
            }
            emit recvMessage(message);


        }else
        {
            if(list[0]=="login")
            {
                if(list[1].split("'")[0].split("=")[1]=="permit")
                {
                    qDebug()<<"get friends";
                    this->username=list[1].split("'")[1].split("=")[1];
                    friends=QString(list[2]).split("'")[1].split(":");
                    friends.takeLast();
                    if(!in_user_interface)
                    {
                        setUserInterface();
                    }else
                    {
                        QPalette pe;
                        pe.setColor(QPalette::WindowText,Qt::green);
                        label_statue->setPalette(pe);
                        this->statue="online";
                        label_statue->setText(statue);
                    }
                }
                else
                {
                    if(!in_user_interface)
                    {
                        setLoginFailInterface();
                    }
                }
            }else
            {
                if(list[0]=="register")
                {
                    if(list[1].split("'")[0].split('=')[1]=="permit")
                    {
                        //提示注册成功
                        QMessageBox mes(this);
                        mes.setText("注册成功\n账号："+list[1].split("'")[1].split('=')[1]);
                        mes.setWindowTitle(tr(""));
                        mes.exec();
                    }else
                    {
                        QMessageBox mes(this);
                        mes.setText("注册失败");
                        mes.setWindowTitle(tr(""));
                        mes.exec();
                        //注册失败
                    }
                }else
                {
                    if(list[0]=="search")
                    {
                        add->result=list[1];
                        emit getSearchresult();
                    }else
                    {
                        if(list[0]=="add")
                        {
                            QStringList info=list[1].split("'");
                            QMessageBox mes(this);
                            mes.setText(info[1].split("=")[1]+"("+info[0].split("=")[1]+")请求添加您为好友");
                            QPushButton *okButton = mes.addButton(QObject::tr("同意"),QMessageBox::AcceptRole);
                            mes.addButton(QObject::tr("拒绝"), QMessageBox::RejectRole);
                            mes.setWindowTitle(tr(""));
                            mes.exec();
                            if(mes.clickedButton()==okButton)
                            {
                                QByteArray data=QString("result|statue=permit'asker="+info[0].split("=")[1]+"'recv="+account).toUtf8();
                                personalSocket->write(data);
                                personalSocket->flush();
                                QListWidgetItem *Item=new QListWidgetItem();
                                Item->setText(info[1].split("=")[1]+"("+info[0].split("=")[1]+")");
                                widget->addItem(Item);
                            }
                            else
                            {
                                QByteArray data=QString("result|deny").toUtf8();
                                personalSocket->write(data);
                                personalSocket->flush();
                            }
                        }else
                        {
                            if(list[0]=="result")
                            {
                                QStringList templist=list[1].split("'");
                                QString username=templist[0].split("=")[1];
                                QString account = templist[1].split("=")[1];
                                QListWidgetItem *Item=new QListWidgetItem();
                                Item->setText(username+"("+account+")");
                                widget->addItem(Item);
                            }else
                            {
                                if(list[0]=="refresh")
                                {
                                    //refresh|friends’friendname=account:friendname=account:end
                                    friends=list[1].split("'")[1].split(":");
                                    friends.takeLast();
                                    widget->clear();
                                    QListWidgetItem *item;
                                    for(int i=0;i<friends.length();i++)
                                    {
                                        item=new QListWidgetItem();
                                        item->setText(friends[i].split("=")[0]+"("+friends[i].split("=")[1]+")");
                                        widget->addItem(item);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

}

void MainWindow::sendMessage(const QString mess)
{
    QByteArray data;
    qDebug()<<"send a message"+mess;
    data=mess.toUtf8();
    personalSocket->write(data);
    personalSocket->flush();
}

void MainWindow::readyToConnect()
{
    personalPort=getportThread->personalPort;
    personalSocket->connectToHost(serverIP,personalPort);
    if(!personalSocket->waitForConnected(5000))
    {
        getportThread->start();
        qDebug()<<"连接到个人端口失败";
        //提示重连
    }
    else
    {
        //thread()->sleep(1);
        qDebug()<<"连接服务器成功";
        this->statue="online";
        getportThread->terminate();
        if(is_registing)
        {
            btn_confirm->setDisabled(false);
        }
        else
        {
            if(in_user_interface)
            {
                QByteArray data;
                QString mess="login|account="+account+"'password="+password+"'mac="+getHostMacAddress();;
                qDebug()<<"登录信息已发送:"+mess;
                data=mess.toUtf8();
                personalSocket->write(data);
                personalSocket->flush();
            }
            else
            {
                QPalette pe;
                pe.setColor(QPalette::WindowText,Qt::green);
                label_statue->setPalette(pe);
                label_statue->setText(statue);
                btn_login->setDisabled(false);
                btn_register->setDisabled(false);
            }
        }

        delete getportThread;

    }
}

void MainWindow::portdisconnect()
{
    qDebug()<<"disconnect";
    this->statue="offline";
    qDebug()<<is_logining<<is_registing<<in_user_interface;
    if(is_logining)
    {
        qDebug()<<1;
        setLoginFailInterface();
        setBackToInitialInterface();
        QPalette pe;
        pe.setColor(QPalette::WindowText,Qt::red);
        label_statue->setPalette(pe);
        btn_login->setDisabled(true);
        btn_register->setDisabled(true);
        label_statue->setText(this->statue);
        getportThread=new portThread();
        connect(getportThread,&portThread::perosonalPortisReady,this,&MainWindow::readyToConnect,Qt::QueuedConnection);
        getportThread->start();
    }
    else
    {
        if(in_user_interface)
        {
            qDebug()<<2;
            QPalette pe;
            pe.setColor(QPalette::WindowText,Qt::red);
            label_statue->setPalette(pe);
            label_statue->setText(this->statue);
        }
        else
        {
            if(is_registing)
            {
                qDebug()<<3;
                btn_confirm->setDisabled(true);
                getportThread=new portThread();
                connect(getportThread,&portThread::perosonalPortisReady,this,&MainWindow::readyToConnect,Qt::QueuedConnection);
                getportThread->start();
            }
            else
            {
                qDebug()<<4;
                QPalette pe;
                pe.setColor(QPalette::WindowText,Qt::red);
                label_statue->setPalette(pe);
                label_statue->setText(this->statue);
                btn_register->setDisabled(true);
                btn_login->setDisabled(true);
                getportThread=new portThread();
                connect(getportThread,&portThread::perosonalPortisReady,this,&MainWindow::readyToConnect,Qt::QueuedConnection);
                getportThread->start();
             }
        }
    }

}

void MainWindow::timerOfDot()
{
    if(counter==7)
    {
        counter=0;
    }
    else {
        counter++;
    }

    QString message="登录中";
    for(int i=0;i<counter+1;i++)
    {
        message+='.';
    }
    label_logining->setText(message);
}

void MainWindow::itemIsClicked(const QModelIndex &index)
{

    QString friendsname=widget->currentItem()->text().split("(")[0];
    QString friendsaccount=widget->currentItem()->text().split("(")[1].split(")")[0];
    this->list_of_dia.append(friendsaccount);
    DialogInterface *dia=new DialogInterface(account, friendsaccount, friendsname);
    dia->username=username;
    dia->widget=this->widget;
    dia->accountlist=&list_of_dia;
    dia->show();
    connect(this,&MainWindow::recvMessage,dia,&DialogInterface::getMessage);
    connect(dia,&DialogInterface::messageIsReady,this,&MainWindow::sendMessage);
}

void MainWindow::addFriend()
{
    add=new AddFriend();
    add->useraccount=account;
    connect(this,&MainWindow::getSearchresult,add,&AddFriend::getSearchResult);
    connect(add,&AddFriend::sendInf,this,&MainWindow::sendMessage);
    add->show();

}

void MainWindow::refresh()
{
    if(statue=="online")
    {
        QByteArray data;
        QString mess="refresh|account="+account;
        data=mess.toUtf8();
        personalSocket->write(data);
        personalSocket->flush();
    }
    else
    {
        getportThread=new portThread();
        getportThread->serverIP=this->serverIP;
        connect(getportThread,&portThread::perosonalPortisReady,this,&MainWindow::readyToConnect,Qt::QueuedConnection);
        getportThread->start();
    }

}

void MainWindow::resetIP()
{

    Setting *set=new Setting(serverIP);
    connect(set,&Setting::ipisready,this,&MainWindow::ipIsReady);
    set->show();
    qDebug()<<"get a error";
}

void MainWindow::ipIsReady(QString ip)
{
    qDebug()<<serverIP;
    serverIP=ip;
    getportThread->serverIP=serverIP;
    getportThread->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//线程类的函数实现
portThread::portThread()
{
    serverIP="127.0.0.1";
    mainPort=10001;


}

void portThread::run()
{
    mainSocket = new QTcpSocket();
    for(int i=0;i<10;i++)
    {
        qDebug()<<"run";
        connect(mainSocket, &QTcpSocket::readyRead,this,&portThread::getPersoanlPort,Qt::DirectConnection);
        mainSocket->connectToHost(serverIP, mainPort);
        if(mainSocket->waitForConnected(5000))
        {     
           qDebug()<<("connect to host");
           break;
        }else
        {
            thread()->sleep(7);
            qDebug()<<QString("第%1重连").arg(i);
            mainSocket->abort();
            mainSocket->connectToHost(serverIP, mainPort);

        }
        //加入提示
    }
    this->exec();
}

void portThread::getPersoanlPort()
{
    qDebug()<<currentThreadId();
    QByteArray array=mainSocket->readAll();
    if(!array.isEmpty())
    {
        qDebug()<<"私人端口号已接收";
        personalPort=tr(array).toInt();
        emit perosonalPortisReady();
        mainSocket->abort();
        delete mainSocket;
        //this->exit(0);
    }
}

portThread::~portThread()
{

}

QString getHostMacAddress()
{
    QList<QNetworkInterface> nets = QNetworkInterface::allInterfaces();// 获取所有网络接口列表
    int nCnt = nets.count();
    QString strMacAddr = "";
    for(int i = 0; i < nCnt; i ++)
    {
        // 如果此网络接口被激活并且正在运行并且不是回环地址，则就是我们需要找的Mac地址
        if(nets[i].flags().testFlag(QNetworkInterface::IsUp) && nets[i].flags().testFlag(QNetworkInterface::IsRunning) && !nets[i].flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            strMacAddr = nets[i].hardwareAddress();
            break;
        }
    }
    return strMacAddr;
}



