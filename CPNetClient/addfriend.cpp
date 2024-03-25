#include "addfriend.h"
#include "ui_addfriend.h"
#include "QStringList"
#include "QMessageBox"

AddFriend::AddFriend(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriend)
{
    ui->setupUi(this);

    btn_search=ui->pushButton;
    edi_acc=ui->lineEdit;
    edi_result=ui->textEdit;
}

void AddFriend::getSearchResult()
{//username=username’account=account”
    QStringList  message=result.split("'");
    QString message_sum="";
    for(int i=0;i<message.length();i++)
    {
        message_sum+=message[i].split("=")[0]+"="+message[i].split("=")[1]+"\n\r";
    }

    edi_result->setText(message_sum);

}
AddFriend::~AddFriend()
{
    delete ui;
}

void AddFriend::on_pushButton_clicked()
{
    QString mess="search|account="+edi_acc->text();
    emit sendInf(mess);
}

void AddFriend::on_pushButton_2_clicked()
{
    QString mess ="add|sender="+useraccount+"'account="+edi_acc->text();
    QMessageBox mes(this);
    mes.setText(QObject::tr("请求已发送"));
    mes.setWindowTitle("");
    mes.exec();
    emit sendInf(mess);
}
