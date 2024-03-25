#include "setting.h"
#include "ui_setting.h"

Setting::Setting(QString ip, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Setting)
{
    ui->setupUi(this);
    this->Ip=ip;
    this->ui->lineEdit->setText(Ip);
}

Setting::~Setting()
{
    delete ui;
}

void Setting::on_pushButton_clicked()
{
    Ip=this->ui->lineEdit->text();
    emit ipisready(Ip);
    this->close();
}
