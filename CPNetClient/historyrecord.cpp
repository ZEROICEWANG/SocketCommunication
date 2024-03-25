#include "historyrecord.h"
#include "ui_historyrecord.h"
#include "QTextEdit"

HistoryRecord::HistoryRecord(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistoryRecord)
{
    ui->setupUi(this);

}

HistoryRecord::HistoryRecord(QWidget *parent,QString record)
{
    ui->setupUi(this);
    QTextEdit *edi_record=new QTextEdit(this);
    edi_record->setGeometry(0,0,this->frameGeometry().width(),this->frameGeometry().height());
    edi_record->setText(record);
    edi_record->setFocusPolicy(Qt::NoFocus);
}

HistoryRecord::~HistoryRecord()
{
    delete ui;
}
