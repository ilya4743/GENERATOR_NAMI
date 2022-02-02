#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_DataBtn_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.txt");
    ui->DataLEdt->setText(str);
}


void Dialog::on_ConfigBtn_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.txt");
    ui->ConfigLEdt->setText(str);
}


void Dialog::on_BPRBtn_clicked()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.txt");
    ui->BPRLEdt->setText(str);
}
