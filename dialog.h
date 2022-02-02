#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileDialog>
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_2_clicked();

    void on_DataBtn_clicked();

    void on_ConfigBtn_clicked();

    void on_BPRBtn_clicked();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
