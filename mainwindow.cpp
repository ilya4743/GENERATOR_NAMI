#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QRegExpValidator>
#include<QFile>
#include"mytcpsocket.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    socket=new MyTcpSocket();
    socket->doConnect();
    ui->setupUi(this);
    //QRegExp rx( "(?<!\d)-?\d*[.,]?\d+" );
    //QValidator *validator = new QRegExpValidator(rx, this);
    vector<QString> vec_str_param;
    vector<QString> vec_str_data;

    //читаем наименование параметров
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

    //читаем значение параметров
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

    //подставляем значение параметров в виджеты
    this->vec_line_edit.reserve(9);
    this->vec_label.reserve(9);
    for(unsigned int i=0; i<9; i++)
    {
        this->vec_line_edit.push_back(new QLineEdit());
        this->vec_line_edit[i]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        this->vec_line_edit[i]->setText(vec_str_data[i]);

        //this->vec_line_edit [i]->setValidator(validator);
        this->vec_label.push_back(new QLabel(vec_str_param[i]));
        ui->formLayout->addWidget(this->vec_label[i],i,0);
        vec_label[i]->setAlignment(Qt::AlignRight|Qt::AlignBottom);

        ui->formLayout->addWidget(this->vec_line_edit[i],i,1);
        //ui->formLayout->addRow(this->vec_label[i], this->vec_line_edit[i]);
    }

    //добавим comboBox для выбора положения center
    ui->formLayout->addWidget(&qComboBox,3,3);
    qComboBox.setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    qComboBox.addItem("Вручную");
    qComboBox.addItem("По центру");
    qComboBox.addItem("По центру cнизу");
    qComboBox.addItem("По центру слева");
    qComboBox.addItem("По центру справа");
    qComboBox.addItem("По центру сверху");


    for(unsigned int i=9; i<vec_str_param.size();i++)
    {
        this->vec_line_edit.push_back(new QLineEdit());
        this->vec_line_edit[i]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        this->vec_line_edit[i]->setText(vec_str_data[i]);

        //this->vec_line_edit[i]->setValidator(validator);
        this->vec_label.push_back(new QLabel(vec_str_param[i]));
        ui->formLayout_4->addWidget(this->vec_label[i]);
        ui->formLayout_4->addRow(this->vec_label[i], this->vec_line_edit[i]);
    }

    if(vec_line_edit[8]->text().toInt()==0)
        for(unsigned int i=9; i<vec_str_param.size();i++)
            vec_line_edit[i]->setEnabled(false);

    connect(this->vec_line_edit[8], SIGNAL(textChanged(const QString &)),this, SLOT(textChanged(const QString &)));
    connect(this->vec_line_edit[9], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier1(const QString &)));
    connect(this->vec_line_edit[10], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier2(const QString &)));
    connect(this->vec_line_edit[11], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier3(const QString &)));
    connect(this->vec_line_edit[12], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier4(const QString &)));

    for(int i=0; i<3; i++)
        connect(this->vec_line_edit[i], SIGNAL(textChanged(const QString &)),this, SLOT(textChanged1(const QString &)));

    connect(&qComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexCenterChanged(int )));

    connect(socket, SIGNAL(data_received(QDataStream &)),this,SLOT(data_send(QDataStream &)));

    goal_point=(Point(vec_str_data[6].toFloat(),vec_str_data[7].toFloat()));
}

void MainWindow::data_send(QDataStream &stream)
{
    float x, y;
    stream>>x>>y;
    recalculateData(Point(x,y));
    this->on_pushButton_clicked();
}

void MainWindow::textChanged1(const QString &s)
{
    int width, height;
    width=vec_line_edit[0]->text().toFloat()/vec_line_edit[2]->text().toFloat();
    height=vec_line_edit[1]->text().toFloat()/vec_line_edit[2]->text().toFloat();
    switch(qComboBox.currentIndex())
    {
        //case 0:
       //     vec_line_edit[3]->setEnabled(true);
        //break;

        case 1:
            //vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2) + (height/2)*(width)).toString());
        break;

        case 2:
            //vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2) + (height-1)*(width)).toString());
        break;

        case 3:
            //vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((height/2)*(width)).toString());
        break;

        case 4:
            //vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width-1) + (height/2)*(width)).toString());
        break;

        case 5:
            //vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2)).toString());
        break;
    }
}

void MainWindow::currentIndexCenterChanged(int index)
{
    int width, height;
    width=vec_line_edit[0]->text().toFloat()/vec_line_edit[2]->text().toFloat();
    height=vec_line_edit[1]->text().toFloat()/vec_line_edit[2]->text().toFloat();
    switch(index)
    {
        case 0:
            vec_line_edit[3]->setEnabled(true);
        break;

        case 1:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2) + (height/2)*(width)).toString());
        break;

        case 2:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2) + (height-1)*(width)).toString());
        break;

        case 3:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((height/2)*(width)).toString());
        break;

        case 4:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width-1) + (height/2)*(width)).toString());
        break;

        case 5:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2)).toString());

        break;
    }
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
        for(unsigned int i=9; i<vec_line_edit.size();i++)
            vec_line_edit[i]->setEnabled(false);
        vec_barrier.clear();
    }
    else
    {
        for(unsigned int i=9; i<vec_line_edit.size();i++)
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

    if(socket->auto_send==false)
    {
        socket->auto_mode(true);
        socket->wnd=this;
        goal_point.x=vec_line_edit[6]->text().toFloat();
        goal_point.y=vec_line_edit[7]->text().toFloat();
        makePack();
    }
    else
    {
        socket->auto_mode(false);
    }
}

void MainWindow::on_action_triggered()
{
    vector<QString> vec_str_data;
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.txt");
    QFile file2(str);
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

    for(int i=0; i<9; i++)
        vec_line_edit[i]->setText(vec_str_data[i]);

    qComboBox.setCurrentIndex(0);
    ui->comboBox->clear();
    vec_barrier.clear();
    vec_barrier.resize(vec_str_data[8].toInt());
    for(int i=0; i<vec_str_data[8].toInt();i++)
    {
        ui->comboBox->addItem("Препятствие " + QVariant(i+1).toString());
        vec_barrier[i].x=vec_str_data[9+i*4].toFloat();
        vec_barrier[i].y=vec_str_data[9+i*4+1].toFloat();
        vec_barrier[i].width=vec_str_data[9+i*4+2].toFloat();
        vec_barrier[i].height=vec_str_data[9+i*4+3].toFloat();
    }
    vec_line_edit[9] ->setText(QString::number(vec_barrier[0].x));
    vec_line_edit[10]->setText(QString::number(vec_barrier[0].y));
    vec_line_edit[11]->setText(QString::number(vec_barrier[0].width));
    vec_line_edit[12]->setText(QString::number(vec_barrier[0].height));
}

void MainWindow::recalculateData(Point p)
{
    for(unsigned int i=0; i<vec_buf_barrier.size();i++)
    {
        vec_barrier[i].x=vec_barrier[i].x-p.x;
        vec_barrier[i].y=vec_barrier[i].y-p.y;
    }
    goal_point.x=goal_point.x-p.x;
    goal_point.y=goal_point.y-p.y;
}

void MainWindow::makePack()
{        
    QByteArray arr;
    QDataStream dataStream(&arr, QIODevice::WriteOnly);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream<<(unsigned char)0x44<<(unsigned char)0x47;
    for(int i=0; i<3; i++)
        dataStream<<vec_line_edit[i]->text().toFloat();

    dataStream<<vec_line_edit[3]->text().toInt();

    for(int i=4; i<=5; i++)
        dataStream<<vec_line_edit[i]->text().toFloat();

    dataStream<<goal_point.x<<goal_point.y;
    dataStream<<vec_line_edit[8]->text().toInt();

    for(unsigned int i=0; i<vec_barrier.size();i++)
        dataStream<<vec_barrier[i].x<<vec_barrier[i].y<<vec_barrier[i].width<<vec_barrier[i].height;
    socket->sendData(arr);
}
