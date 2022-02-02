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
    QFile file1("param.txt");
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
    QFile file2("data.txt");
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
    vec_line_edit.reserve(9);
    vec_label.reserve(9);
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
        vec_line_edit.push_back(new QLineEdit());
        vec_line_edit[i]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        vec_line_edit[i]->setText(vec_str_data[i]);

        //this->vec_line_edit[i]->setValidator(validator);
        vec_label.push_back(new QLabel(vec_str_param[i]));
        ui->formLayout_4->addWidget(this->vec_label[i]);
        ui->formLayout_4->addRow(this->vec_label[i], this->vec_line_edit[i]);
    }

    vec_check_box.push_back(new QCheckBox("Симуляция движения"));
    vec_check_box.push_back(new QCheckBox("Зациклить"));
    vec_check_box.push_back(new QCheckBox("Точка маршрута недостижима"));
    vec_check_box.push_back(new QCheckBox("Ожидать ответа БПР"));
    vec_check_box[3]->setCheckState(Qt::CheckState::Checked);
    isWaitBPR=true;
    for(unsigned int i=0; i<vec_check_box.size();i++)
    {
        ui->formLayout_4->addWidget(this->vec_check_box[i]);
        ui->formLayout_4->addRow(this->vec_check_box[i]);
    }
    vec_check_box[1]->setEnabled(false);
    vec_check_box[2]->setEnabled(false);
    vec_check_box[3]->setEnabled(false);
    connect(vec_check_box[0],SIGNAL(stateChanged(int)),this,SLOT(motionSimulation(int)));
    connect(vec_check_box[1],SIGNAL(stateChanged(int)),this,SLOT(loopSimulation(int)));
    connect(vec_check_box[2],SIGNAL(stateChanged(int)),this,SLOT(endlessSimulation(int)));
    connect(vec_check_box[3],SIGNAL(stateChanged(int)),this,SLOT(waitBPR(int)));

    if(vec_line_edit[8]->text().toInt()==0)
        for(unsigned int i=9; i<vec_str_param.size();i++)
            vec_line_edit[i]->setEnabled(false);

    vec_line_edit.push_back(new QLineEdit());
    vec_line_edit[vec_line_edit.size()-1]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    vec_line_edit[vec_line_edit.size()-1]->setEnabled(false);
    vec_line_edit[vec_line_edit.size()-1]->setText("1000");
    vec_label.push_back(new QLabel("Задержка мсек"));
    vec_label[vec_label.size()-1]->setEnabled(false);
    ui->formLayout_4->addRow(vec_label[vec_label.size()-1],vec_line_edit[vec_line_edit.size()-1]);

    stop_btn=new QPushButton("Остановить симуляцию",this);
    connect(stop_btn,SIGNAL(clicked()),this, SLOT(stopSimulationBtn_click()));
    stop_btn->setEnabled(false);
    ui->formLayout_4->addRow(stop_btn);

    connect(vec_line_edit[8], SIGNAL(textChanged(const QString &)),this, SLOT(textChanged(const QString &)));
    connect(vec_line_edit[9], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier1(const QString &)));
    connect(vec_line_edit[10], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier2(const QString &)));
    connect(vec_line_edit[11], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier3(const QString &)));
    connect(vec_line_edit[12], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier4(const QString &)));

    for(int i=0; i<3; i++)
        connect(vec_line_edit[i], SIGNAL(textChanged(const QString &)),this, SLOT(textChanged1(const QString &)));

    connect(&qComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexCenterChanged(int )));

    //goal_point=(Point(vec_str_data[6].toFloat(),vec_str_data[7].toFloat()));
}

void MainWindow::waitBPR(int state)
{
    if(state==2)
        isWaitBPR=true;
    else if(state==0)
        isWaitBPR=false;
}

void MainWindow::stopSimulationBtn_click()
{
    stop_btn->setEnabled(false);
    socket->auto_mode(false);
    ui->sendDataBtn->setEnabled(true);
    stopSimulation=true;
}

void MainWindow::endlessSimulation(int state)
{
    if(state==2)
        isEndlessSimulation=true;
    else if(state==0)
        isEndlessSimulation=false;
    vec_check_box[3]->setEnabled(true);
}

void MainWindow::loopSimulation(int state)
{
    if(state==2)
        isLoopSimulation=true;
    else if(state==0)
        isLoopSimulation=false;
}

void MainWindow::motionSimulation(int state)
{
    if(state==2)
    {
        isMotionSimulation=true;
        //socket->auto_mode(true);
        socket->wnd=this;
        vec_check_box[1]->setEnabled(true);
        vec_check_box[2]->setEnabled(true);
        vec_check_box[3]->setEnabled(true);
        vec_line_edit[vec_line_edit.size()-1]->setEnabled(true);
        vec_label[vec_label.size()-1]->setEnabled(true);
    }
    else if(state==0)
    {
        isMotionSimulation=false;
        //socket->auto_mode(false);
        vec_check_box[1]->setEnabled(false);
        vec_check_box[1]->setCheckState(Qt::CheckState::Unchecked);
        vec_check_box[2]->setEnabled(false);
        vec_check_box[2]->setCheckState(Qt::CheckState::Unchecked);
        vec_check_box[3]->setEnabled(false);
        vec_line_edit[vec_line_edit.size()-1]->setEnabled(false);
        vec_label[vec_label.size()-1]->setEnabled(false);
    }
}

void MainWindow::textChanged1(const QString &s)
{
    int width, height;
    width=vec_line_edit[0]->text().toFloat()/vec_line_edit[2]->text().toFloat();
    height=vec_line_edit[1]->text().toFloat()/vec_line_edit[2]->text().toFloat();
    switch(qComboBox.currentIndex())
    {
        case 1:
            vec_line_edit[3]->setText(QVariant((width / 2) + (height/2)*(width)).toString());
        break;

        case 2:
            vec_line_edit[3]->setText(QVariant((width / 2) + (height-1)*(width)).toString());
        break;

        case 3:
            vec_line_edit[3]->setText(QVariant((height/2)*(width)).toString());
        break;

        case 4:
            vec_line_edit[3]->setText(QVariant((width-1) + (height/2)*(width)).toString());
        break;

        case 5:
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
    for(auto it= vec_check_box.begin(); it!=vec_check_box.end(); ++it)
        delete(*it);
    vec_check_box.clear();
    delete stop_btn;
    if (settings_wnd!=NULL)
        delete settings_wnd;
}


void MainWindow::on_ExitBtn_triggered()
{
    QApplication::quit();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    vec_line_edit[9] ->setText(QString::number(vec_barrier[index].x));
    vec_line_edit[10]->setText(QString::number(vec_barrier[index].y));
    vec_line_edit[11]->setText(QString::number(vec_barrier[index].width));
    vec_line_edit[12]->setText(QString::number(vec_barrier[index].height));
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

void MainWindow::recalculateBarrier(Point p)
{
    for(unsigned int i=0; i<vec_buf_barrier.size();i++)
    {
        vec_buf_barrier[i].x=vec_buf_barrier[i].x-p.x;
        vec_buf_barrier[i].y=vec_buf_barrier[i].y-p.y;
    }
}

void MainWindow::recalculateGoal(Point p)
{
    goal_point_buf.x=goal_point_buf.x-p.x;
    goal_point_buf.y=goal_point_buf.y-p.y;
}

void MainWindow::recalculateGoalX(float x)
{
    goal_point_buf.x=goal_point_buf.x-x;
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

    dataStream<<goal_point_buf.x<<goal_point_buf.y;
    dataStream<<vec_line_edit[8]->text().toInt();

    for(unsigned int i=0; i<vec_buf_barrier.size();i++)
        dataStream<<vec_buf_barrier[i].x<<vec_buf_barrier[i].y<<vec_buf_barrier[i].width<<vec_buf_barrier[i].height;
    socket->sendData(arr);
}

void MainWindow::on_sendDataBtn_clicked()
{
    goal_point.x=vec_line_edit[6]->text().toFloat();
    goal_point.y=vec_line_edit[7]->text().toFloat();
    vec_buf_barrier=vec_barrier;
    goal_point_buf=goal_point;
    socket->auto_mode(isMotionSimulation);
    //если нет симуляции движения
    if(!isMotionSimulation)
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
    //если есть симуляция движения
    else
    {
        socket->SetDelay_time(vec_line_edit[vec_line_edit.size()-1]->text().toInt());
        //если не ожидаем БПР
        if(!isWaitBPR)
        {
            stop_btn->setEnabled(true);
            ui->sendDataBtn->setEnabled(false);
            while(!stopSimulation)
            {
                makePack();
            }

        }
        else
        {
            //если движение зациклено или точка маршрута недостижима
            if(vec_check_box[1]->checkState()==Qt::CheckState::Checked||vec_check_box[2]->checkState()==Qt::CheckState::Checked)
            {
                stop_btn->setEnabled(true);
                ui->sendDataBtn->setEnabled(false);
            }
            //если движение зациклено и точка маршрута недостижима
            if(vec_check_box[1]->checkState()==Qt::CheckState::Checked&&vec_check_box[2]->checkState()==Qt::CheckState::Checked)
                count_line=40;
            makePack();
        }
    }
}
#include "dialog.h"

void MainWindow::on_SettingsBtn_triggered()
{
    //settings_wnd=new Settings();
    //settings_wnd->show();
    Dialog *d=new Dialog();
    d->show();
}

