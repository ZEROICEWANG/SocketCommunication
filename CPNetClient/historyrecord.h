#ifndef HISTORYRECORD_H
#define HISTORYRECORD_H

#include <QDialog>
#include "QString"

namespace Ui {
class HistoryRecord;
}

class HistoryRecord : public QDialog
{
    Q_OBJECT

public:
    explicit HistoryRecord(QWidget *parent = 0);
    HistoryRecord(QWidget *parent,QString record);
    ~HistoryRecord();

private:
    Ui::HistoryRecord *ui;


};

#endif // HISTORYRECORD_H
