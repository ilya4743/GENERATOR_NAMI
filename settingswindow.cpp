#include "settingswindow.h"
#include "ui_settings.h"
 #include <QDebug>

SettingsWindow::SettingsWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_lineEdit_textEdited(const QString &arg1)
{
    qDebug()<<"line edited";
}


void SettingsWindow::on_lineEdit_inputRejected()
{
    qDebug()<<"input Rejected";

}

