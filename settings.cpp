#include "settings.h"
#include "ui_settings.h"
 #include <QDebug>

Settings::Settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_lineEdit_textEdited(const QString &arg1)
{
    qDebug()<<"line edited";
}


void Settings::on_lineEdit_inputRejected()
{
    qDebug()<<"input Rejected";

}

