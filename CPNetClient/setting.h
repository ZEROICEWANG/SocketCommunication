#ifndef SETTING_H
#define SETTING_H

#include <QDialog>
namespace Ui {
class Setting;
}

class Setting : public QDialog
{
    Q_OBJECT

public:
    explicit Setting(QString ip, QWidget *parent = 0);
    ~Setting();
    QString Ip;

private:
    Ui::Setting *ui;
signals:
    void ipisready(QString ip);
private slots:
    void on_pushButton_clicked();
};

#endif // SETTING_H
