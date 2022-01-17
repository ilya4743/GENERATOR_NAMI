#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegExpValidator>
#include<QFile>
#include"mytcpsocket.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    socket=new MyTcpSocket();
    socket->doConnect();
    ui->setupUi(this);
    //QRegExp rx( "(?<!\d)-?\d*[.,]?\d+" );
    //QValidator *validator = new QRegExpValidator(rx, this);
    vector<QString> vec_str_param;
    vector<QString> vec_str_data;

    QFile file1("/home/NAMI/ila.solomatin/untitled7/param.txt");
    if ((file1.exists())&&(file1.open(QIODevice::ReadOnly)))
    {
        QString str="";
        while(!file1.atEnd())
        {
            str=str+file1.readLine();
            vec_str_param.push_back(str);
            str="";
        }
        file1.close();
    }

    QFile file2("/home/NAMI/ila.solomatin/untitled7/data.txt");
    if ((file2.exists())&&(file2.open(QIODevice::ReadOnly)))
    {
        QString str="";
        while(!file2.atEnd())
        {
            str=str+file2.readLine();
            vec_str_data.push_back(str);
            str="";
        }
        file2.close();
    }

    this->vec_line_edit.reserve(9);
    this->vec_label.reserve(9);
    for(int i=0; i<9; i++)
    {
        this->vec_line_edit.push_back(new QLineEdit());
        this->vec_line_edit[i]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        this->vec_line_edit[i]->setText(vec_str_data[i]);

        //this->vec_line_edit[i]->setValidator(validator);
        this->vec_label.push_back(new QLabel(vec_str_param[i]));
        ui->formLayout->addWidget(this->vec_label[i]);
        ui->formLayout->addRow(this->vec_label[i], this->vec_line_edit[i]);
    }
    //this->vec_line_edit[8].
    connect(this->vec_line_edit[8], SIGNAL(textChanged()),this, SLOT(textChanged()));
}

void MainWindow::textChanged()
{
    ui->comboBox->clear();
    for(int i=0; i<vec_line_edit[8]->text().toInt();)
}

MainWindow::~MainWindow()
{
    delete ui;
    for(auto it= vec_label.begin(); it!=vec_label.end(); ++it)
        delete(*it);
    vec_label.clear();
    for(auto it= vec_line_edit.begin(); it!=vec_line_edit.end(); ++it)
        delete(*it);
    vec_line_edit.clear();
    delete socket;
}


void MainWindow::on_ExitBtn_triggered()
{
    QApplication::quit();
}


void MainWindow::on_pushButton_clicked()
{
    QByteArray arr;
    QDataStream dataStream(&arr, QIODevice::WriteOnly);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream<<(unsigned char)0x44<<(unsigned char)0x47;
    for(int i=0; i<3; i++)
        dataStream<<vec_line_edit[i]->text().toFloat();

    dataStream<<vec_line_edit[3]->text().toInt();

    for(int i=4; i<vec_line_edit.size()-1; i++)
        dataStream<<vec_line_edit[i]->text().toFloat();

    dataStream<<vec_line_edit[vec_line_edit.size()-1]->text().toInt();

    socket->sendData(arr);
}


void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{

}

