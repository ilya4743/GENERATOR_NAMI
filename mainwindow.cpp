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

    for(int i=9; i<vec_str_param.size();i++)
    {
        this->vec_line_edit.push_back(new QLineEdit());
        this->vec_line_edit[i]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        this->vec_line_edit[i]->setText("0");

        //this->vec_line_edit[i]->setValidator(validator);
        this->vec_label.push_back(new QLabel(vec_str_param[i]));
        ui->formLayout_4->addWidget(this->vec_label[i]);
        ui->formLayout_4->addRow(this->vec_label[i], this->vec_line_edit[i]);
    }

    if(vec_line_edit[8]->text().toInt()==0)
        for(int i=9; i<vec_str_param.size();i++)
            vec_line_edit[i]->setEnabled(false);

    connect(this->vec_line_edit[8], SIGNAL(textChanged(const QString &)),this, SLOT(textChanged(const QString &)));
    connect(this->vec_line_edit[9], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier1(const QString &)));
    connect(this->vec_line_edit[10], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier2(const QString &)));
    connect(this->vec_line_edit[11], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier3(const QString &)));
    connect(this->vec_line_edit[12], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier4(const QString &)));

}

void MainWindow::textChangedBarrier1(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].x=arg1.toFloat();
}

void MainWindow::textChangedBarrier2(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].y=arg1.toFloat();
}

void MainWindow::textChangedBarrier3(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].width=arg1.toFloat();
}

void MainWindow::textChangedBarrier4(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].height=arg1.toFloat();
}

void MainWindow::textChanged(const QString &arg1)
{

    if(vec_line_edit[8]->text().toInt()==0)
    {
        for(int i=9; i<vec_line_edit.size();i++)
            vec_line_edit[i]->setEnabled(false);
        vec_barrier.clear();
    }
    else
    {
        for(int i=9; i<vec_line_edit.size();i++)
            vec_line_edit[i]->setEnabled(true);
        vec_barrier.resize(vec_line_edit[8]->text().toInt());
    }
    ui->comboBox->clear();
    for(int i=0; i<arg1.toInt();i++)
        ui->comboBox->addItem("Препятствие " + QVariant(i+1).toString());
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

    for(int i=4; i<8; i++)
        dataStream<<vec_line_edit[i]->text().toFloat();

    dataStream<<vec_line_edit[8]->text().toInt();

    for(unsigned int i=0; i<vec_barrier.size();i++)
        dataStream<<vec_barrier[i].x<<vec_barrier[i].y<<vec_barrier[i].width<<vec_barrier[i].height;

    socket->sendData(arr);
}

void MainWindow::on_lineEdit_textChanged(const QString &arg1)
{

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    vec_line_edit[9] ->setText(QString::number(vec_barrier[index].x));
    vec_line_edit[10]->setText(QString::number(vec_barrier[index].y));
    vec_line_edit[11]->setText(QString::number(vec_barrier[index].width));
    vec_line_edit[12]->setText(QString::number(vec_barrier[index].height));
}


void MainWindow::on_pushButton_2_clicked()
{
    vec_barrier[ind].x=vec_line_edit[9]->text().toFloat();
    vec_barrier[ind].y=vec_line_edit[10]->text().toFloat();
    vec_barrier[ind].width=vec_line_edit[11]->text().toFloat();
    vec_barrier[ind].height=vec_line_edit[12]->text().toFloat();
}
