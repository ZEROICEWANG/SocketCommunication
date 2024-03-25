#ifndef ADDFRIEND_H
#define ADDFRIEND_H

#include <QDialog>
#include "QPushButton"
#include "QLineEdit"
#include "QTextEdit"
#include "QString"

namespace Ui {
class AddFriend;
}

class AddFriend : public QDialog
{
    Q_OBJECT

public:
    explicit AddFriend(QWidget *parent = 0);
    ~AddFriend();

private:
    Ui::AddFriend *ui;
    QPushButton *btn_search;
    QLineEdit *edi_acc;
    QTextEdit *edi_result;
public:
    QString result;
    QString useraccount;
signals:
    void sendInf(const QString Inf);
public slots:
    void getSearchResult();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
};

#endif // ADDFRIEND_H
