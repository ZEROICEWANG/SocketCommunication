#include "dialoginterface.h"
#include "ui_dialoginterface.h"
#include "QSize"
#include "historyrecord.h"
#include "QMessageBox"
#include "QDir"
#include "QFile"
#include "QListWidget"
#include "QTextStream"
#include "QObject"

DialogInterface::DialogInterface(QString useraccount,QString account, QString name,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInterface)
{
    ui->setupUi(this);
    this->friendaccount=account;
    this->friendname=name;
    this->useraccount=useraccount;
    historyRecord="";
    checkFile();

    setFixedSize(QSize(700,700));
    edi_uname=new QLineEdit(this);
    edi_acc=new QLineEdit(this);
    edi_Ip=new QLineEdit(this);
    btn_getIp=new QPushButton(this);
    edi_record=new QTextEdit(this);
    edi_send=new QTextEdit(this);
    btn_sendMessage=new QPushButton(this);
    edi_uname->setText(friendname);
    edi_uname->setFocusPolicy(Qt::NoFocus);
    edi_acc->setText(friendaccount);
    edi_acc->setFocusPolicy(Qt::NoFocus);
    edi_Ip->setText(Ip);
    edi_Ip->setFocusPolicy(Qt::NoFocus);
    btn_getIp->setText("对方IP:mac");
    btn_getIp->setDefault(false);
    btn_sendMessage->setText("发送");
    btn_sendMessage->setDefault(true);
    edi_uname->setGeometry(1,10,170,30);
    edi_acc->setGeometry(181,10,170,30);
    edi_Ip->setGeometry(360,10,200,30);
    btn_getIp->setGeometry(565,10,130,30);

    edi_record->setGeometry(1,50,698,400);
    edi_record->setFocusPolicy(Qt::NoFocus);
    edi_send->setGeometry(1,480,698,220);
    btn_sendMessage->setGeometry(590,650,100,40);

    QPushButton *btn_history=new QPushButton(this);
    btn_history->setText("聊天记录");
    btn_history->setGeometry(490,450,100,30);
    QPushButton *btn_deletefriend=new QPushButton(this);
    btn_deletefriend->setText("删除好友");
    btn_deletefriend->setGeometry(600,450,100,30);


    connect(btn_getIp,&QPushButton::clicked,this,&DialogInterface::getIp);
    connect(btn_sendMessage,&QPushButton::clicked,this,&DialogInterface::sendMessage);
    connect(btn_history,&QPushButton::clicked,this,&DialogInterface::showHistoryRecord);
    connect(btn_deletefriend,&QPushButton::clicked,this,&DialogInterface::deleteFrined);

}

void DialogInterface::getIp()
{
    emit messageIsReady(QString("IP|account="+friendaccount));
}

void DialogInterface::sendMessage()
{
    message="chat|sender="+useraccount+"'account="+friendaccount+"'message="+edi_send->toPlainText();
    emit messageIsReady(message);
    if(record.isEmpty())
    {
        record+="我：\n  "+edi_send->toPlainText();
    }
    else
    {
        record+="\n我：\n  "+edi_send->toPlainText();
    }
    edi_record->setText(record);
    edi_send->setText("");

}

void DialogInterface::showHistoryRecord()
{
    QDir dir("E://CC//"+useraccount);
    dir.setCurrent("E://CC//"+useraccount);
    dir.cd("E://CC//"+useraccount);
    file=new QFile("E://CC//"+useraccount+"//"+friendaccount+"//chatlog.txt");
    file->open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream in(file);
    historyRecord=in.readAll();
    file->close();

    if(historyRecord.isEmpty())
    {
        historyRecord=record;
    }
    else
    {
        if(!record.isEmpty())
        {
            historyRecord=historyRecord+"\n"+record;
        }

    }
    HistoryRecord *history=new HistoryRecord(this,historyRecord);
    history->show();
    file=new QFile("E://CC//"+useraccount+"//"+friendaccount+"//chatlog.txt");
    file->open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream out(file);
    out << historyRecord;
    file->close();
    record="";
    edi_record->setText(record);
}

void DialogInterface::deleteFrined()
{
    QMessageBox mes(this);
    mes.setText(tr("是否删除好友：")+friendname);
    //info[0].split("=")[1]+"("+info[1].split("=")[1]+
    QPushButton *okButton = (mes.addButton(tr("删除"),QMessageBox::AcceptRole));
    mes.addButton(tr("取消"), QMessageBox::RejectRole);
    mes.setWindowTitle(tr(""));
    mes.exec();

    if(mes.clickedButton()==okButton)
    {
        //delete|sender=account’recv= account
        message="delete|sender="+useraccount+"'account="+friendaccount;
        emit messageIsReady(message);
        QDir dir("E://CC"+useraccount);
        dir.remove("E://CC//"+useraccount+"//"+friendaccount+"//chatlog.txt");
        dir.rmpath(dir.absolutePath()+"//"+friendaccount);
        for(int i=0;i<widget->count();i++)
        {
            if(widget->item(i)->text()==friendname+"("+friendaccount+")")
            {
                widget->takeItem(i);
            }
        }
        this->close();
    }
}

void DialogInterface::getMessage(QString mess)
{
    //chat|friendname=friendname’account=account’message=message
    //IP|account=account’ip=XXX.XXX.XXX.XXX
    list=mess.split("|");
    if(list[1].split("'")[1].split("=")[1]==friendaccount)
    {
        if(list[0]=="chat")
        {
            if(record.isEmpty())
            {
                record=friendname+"：\n  "+list[1].split("'")[2].split("=")[1];
            }
            else
            {
                record+=("\n"+friendname+"：\n  "+list[1].split("'")[2].split("=")[1]);
            }
            edi_record->setText(record);
        }
    }

    if(list[1].split("'")[0].split("=")[1]==friendaccount)
    {
        Ip=list[1].split("'")[1].split("=")[1];
        mac=list[1].split("'")[2].split("=")[1];
        edi_Ip->setText(Ip+"||"+mac);
    }

}

void DialogInterface::checkFile()
{
    QString path="E:/CC//"+useraccount+"//"+friendaccount;
    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkdir("E:/CC//"+useraccount+"//"+friendaccount);
    }

}

void DialogInterface::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);

    file=new QFile("E://CC//"+useraccount+"//"+friendaccount+"//chatlog.txt");
    file->open(QIODevice::ReadWrite|QIODevice::Text);
    QTextStream in(file);
    historyRecord=in.readAll();
    file->close();

    if(historyRecord.isEmpty())
    {
        historyRecord=record;
    }
    else
    {
        if(!record.isEmpty())
        {
            historyRecord=historyRecord+"\n"+record;
        }

    }


    for(int i=0;i<accountlist->length();i++)
    {
        if(friendaccount==(*accountlist)[i])
        {
            accountlist->takeAt(i);
            break;
        }
    }
}


DialogInterface::~DialogInterface()
{
    delete ui;
}
