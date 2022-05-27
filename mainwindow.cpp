#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
//#include <QRegExpValidator>
#include<QFile>
#include"mytcpsocket.h"
#include <QFileDialog>
#include "settings.h"

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow)
{
    QFile f("config.cfg");
    f.open(QIODevice::ReadOnly);
    Settings::setDefaults(f.readAll());
    socket=new MyTcpSocket();
    socket->doConnect(Settings::get(Settings::IP_SERVER).toString(),Settings::get(Settings::PORT).toInt(),Settings::get(Settings::RECONNECT_TIME).toInt());
    ui->setupUi(this);
    //QRegExp rx( "(?<!\d)-?\d*[.,]?\d+" );
    //QValidator *validator = new QRegExpValidator(rx, this);
    vector<QString> vec_str_param;

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

    QString b("defaultValues.xml");
    //читаем значение параметров
    countBarriers=0;
    //XMLGenerator::importXML(b,map,car,goal_point,vec_barrier, countBarriers);

    //подставляем значение параметров в виджеты
    vec_line_edit.reserve(9);
    vec_label.reserve(9);
    for(unsigned int i=0; i<8; i++)
    {
        vec_line_edit.push_back(new QLineEdit());
        vec_line_edit[i]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
        vec_label.push_back(new QLabel(vec_str_param[i]));
        ui->formLayout->addWidget(vec_label[i],i,0);
        vec_label[i]->setAlignment(Qt::AlignRight|Qt::AlignBottom);
        ui->formLayout->addWidget(vec_line_edit[i],i,1);
    }
    vec_line_edit.push_back(new QLineEdit());
    vec_line_edit[8]->setSizePolicy(QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed));
    vec_label.push_back(new QLabel(vec_str_param[8]));
    ui->formLayout->addWidget(vec_label[8],8,0);
    vec_label[8]->setAlignment(Qt::AlignRight|Qt::AlignBottom);

    spinBoxN=new QSpinBox;
    ui->formLayout->addWidget(spinBoxN,8,1);
    connect(spinBoxN, SIGNAL(textChanged(const QString &)),this, SLOT(textChanged(const QString &)));
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
        vec_label.push_back(new QLabel(vec_str_param[i]));
        ui->formLayout_4->addWidget(vec_label[i]);
        ui->formLayout_4->addRow(vec_label[i], vec_line_edit[i]);
    }
    vec_check_box.push_back(new QCheckBox("Сглаживание"));
    vec_check_box.push_back(new QCheckBox("Симуляция движения"));
    vec_check_box.push_back(new QCheckBox("Зациклить"));
    vec_check_box.push_back(new QCheckBox("Точка маршрута недостижима"));
    vec_check_box.push_back(new QCheckBox("Ожидать ответа БПР"));
    isWaitBPR=true;
    for(unsigned int i=0; i<vec_check_box.size();i++)
    {
        ui->formLayout_4->addWidget(vec_check_box[i]);
        ui->formLayout_4->addRow(vec_check_box[i]);
    }
    vec_check_box[2]->setEnabled(false);
    vec_check_box[3]->setEnabled(false);
    vec_check_box[4]->setEnabled(false);
    connect(vec_check_box[0],SIGNAL(stateChanged(int)),this,SLOT(smoothPath(int)));
    connect(vec_check_box[1],SIGNAL(stateChanged(int)),this,SLOT(motionSimulation(int)));
    connect(vec_check_box[2],SIGNAL(stateChanged(int)),this,SLOT(loopSimulation(int)));
    connect(vec_check_box[3],SIGNAL(stateChanged(int)),this,SLOT(endlessSimulation(int)));
    connect(vec_check_box[4],SIGNAL(stateChanged(int)),this,SLOT(waitBPR(int)));

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

    //connect(vec_line_edit[8], SIGNAL(textChanged(const QString &)),this, SLOT(textChanged(const QString &)));
    connect(vec_line_edit[9],  SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier1(const QString &)));
    connect(vec_line_edit[10], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier2(const QString &)));
    connect(vec_line_edit[11], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier3(const QString &)));
    connect(vec_line_edit[12], SIGNAL(textChanged(const QString &)),this, SLOT(textChangedBarrier4(const QString &)));

    for(int i=0; i<3; i++)
        connect(vec_line_edit[i], SIGNAL(textChanged(const QString &)),this, SLOT(textChanged1(const QString &)));

    connect(&qComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(currentIndexCenterChanged(int )));

    isMotionSimulation=false;
    isLoopSimulation=false;
    isEndlessSimulation=false;
    isWaitBPR=false;
    stopSimulation=false;
    isSmoothing=false;
    updateWidgets();
    window=new Visualizer;
    window->setWindowTitle("Visualizer");
    window->setGeometry(100,100,500,500); // Смещение и положение окна
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
    disconnect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadEndlessSimulation()));
    connect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadNoSimulation()));
    disconnect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadSimpleSimulation()));
    ui->sendDataBtn->setEnabled(true);
    stopSimulation=true;
    if(!isWaitBPR)
    {
        qDebug()<<"Pack "<<countSendPack<<" send";
        qDebug()<<"Pack "<<countReceivePack<<" recieve";
    }
}

void MainWindow::endlessSimulation(int state)
{
    if(state==2)
    {
        isEndlessSimulation=true;
        disconnect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadSimpleSimulation()));
        connect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadEndlessSimulation()));
    }
    else if(state==0)
    {
        isEndlessSimulation=false;
        disconnect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadEndlessSimulation()));
        connect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadSimpleSimulation()));
    }
    vec_check_box[4]->setEnabled(true);
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
        socket->wnd=this;
        vec_check_box[2]->setEnabled(true);
        vec_check_box[3]->setEnabled(true);
        vec_check_box[4]->setEnabled(true);
        vec_line_edit[vec_line_edit.size()-1]->setEnabled(true);
        vec_label[vec_label.size()-1]->setEnabled(true);
        disconnect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadNoSimulation()));
        connect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadSimpleSimulation()));
    }
    else if(state==0)
    {
        isMotionSimulation=false;
        vec_check_box[2]->setEnabled(false);
        vec_check_box[2]->setCheckState(Qt::CheckState::Unchecked);
        vec_check_box[3]->setEnabled(false);
        vec_check_box[3]->setCheckState(Qt::CheckState::Unchecked);
        vec_check_box[4]->setEnabled(false);
        vec_line_edit[vec_line_edit.size()-1]->setEnabled(false);
        vec_label[vec_label.size()-1]->setEnabled(false);
        disconnect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadSimpleSimulation()));
        connect(socket->GetQTcpSocket(), SIGNAL(readyRead()),socket, SLOT(readyReadNoSimulation()));
    }
}

void MainWindow::smoothPath(int state)
{
    if(state==2)
        isSmoothing=true;
    else if(state==0)
        isSmoothing=false;
    socket->changeModeBPR(isSmoothing);
}

void MainWindow::textChanged1(const QString &s)
{
    int width, height;
    width=map.width/map.step;
    height=map.height/map.step;
    switch(qComboBox.currentIndex())
    {
        case 1:
            vec_line_edit[3]->setText(QVariant((width / 2) + (height/2)*(width)).toString());
            map.center=(width / 2) + (height/2)*(width);
        break;

        case 2:
            vec_line_edit[3]->setText(QVariant((width / 2) + (height-1)*(width)).toString());
            map.center=(width / 2) + (height-1)*(width);
        break;

        case 3:
            vec_line_edit[3]->setText(QVariant((height/2)*(width)).toString());
            map.center=(height/2)*(width);
        break;

        case 4:
            vec_line_edit[3]->setText(QVariant((width-1) + (height/2)*(width)).toString());
            map.center=(width-1) + (height/2)*(width);
        break;

        case 5:
            vec_line_edit[3]->setText(QVariant((width / 2)).toString());
            map.center=(width / 2);
        break;
    }
}

void MainWindow::currentIndexCenterChanged(int index)
{
    int width, height;
    map.width=vec_line_edit[0]->text().toInt();
    map.height=vec_line_edit[1]->text().toInt();
    width=map.width/map.step;
    height=map.height/map.step;
    switch(index)
    {
        case 0:
            vec_line_edit[3]->setEnabled(true);
            map.center=vec_line_edit[3]->text().toInt();
        break;

        case 1:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2) + (height/2)*(width)).toString());
            map.center=(width / 2) + (height/2)*(width);
        break;

        case 2:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2) + (height-1)*(width)).toString());
            map.center=(width / 2) + (height-1)*(width);
        break;

        case 3:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((height/2)*(width)).toString());
            map.center=(height/2)*(width);
        break;

        case 4:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width-1) + (height/2)*(width)).toString());
            map.center=(width-1) + (height/2)*(width);
        break;

        case 5:
            vec_line_edit[3]->setEnabled(false);
            vec_line_edit[3]->setText(QVariant((width / 2)).toString());
            map.center=(width / 2);
        break;
    }
}

void MainWindow::textChangedBarrier1(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].position.x=arg1.toFloat();
}

void MainWindow::textChangedBarrier2(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].position.y=arg1.toFloat();
}

void MainWindow::textChangedBarrier3(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].scale.x=arg1.toFloat();
}

void MainWindow::textChangedBarrier4(const QString &arg1)
{
    vec_barrier[ui->comboBox->currentIndex()].scale.y=arg1.toFloat();
}

void MainWindow::textChanged(const QString &arg1)
{
    if(arg1.toInt()==0)
    {
        for(unsigned int i=9; i<vec_line_edit.size();i++)
            vec_line_edit[i]->setEnabled(false);
        vec_barrier.clear();
    }
    else
    {
        for(unsigned int i=9; i<vec_line_edit.size();i++)
            vec_line_edit[i]->setEnabled(true);
        vec_barrier.resize(arg1.toInt());
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
    delete spinBoxN;

    delete window;
}


void MainWindow::on_ExitBtn_triggered()
{
    QApplication::quit();
}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    vec_line_edit[9] ->setText(QString::number(vec_barrier[index].position.x));
    vec_line_edit[10]->setText(QString::number(vec_barrier[index].position.y));
    vec_line_edit[11]->setText(QString::number(vec_barrier[index].scale.x));
    vec_line_edit[12]->setText(QString::number(vec_barrier[index].scale.y));
}

void MainWindow::updateWidgets()
{
    vec_line_edit[0]->setText(QString::number(map.width));
    vec_line_edit[1]->setText(QString::number(map.height));
    vec_line_edit[2]->setText(QString::number(map.step));
    vec_line_edit[3]->setText(QString::number(map.center));

    vec_line_edit[4]->setText(QString::number(car.scale.x));
    vec_line_edit[5]->setText(QString::number(car.scale.y));

    vec_line_edit[6]->setText(QString::number(goal_point.x));
    vec_line_edit[7]->setText(QString::number(goal_point.y));

    spinBoxN->setValue(countBarriers);
    qComboBox.setCurrentIndex(0);
    ui->comboBox->clear();
    for(int i=0; i<countBarriers;i++)
        ui->comboBox->addItem("Препятствие " + QVariant(i+1).toString());

    if(countBarriers>0)
    {
        vec_line_edit[9] ->setText(QString::number(vec_barrier[0].position.x));
        vec_line_edit[10]->setText(QString::number(vec_barrier[0].position.y));
        vec_line_edit[11]->setText(QString::number(vec_barrier[0].scale.x));
        vec_line_edit[12]->setText(QString::number(vec_barrier[0].scale.y));
    }
}

void MainWindow::updateValues()
{
    map.width=vec_line_edit[0]->text().toFloat();
    map.height=vec_line_edit[1]->text().toFloat();
    map.step=vec_line_edit[2]->text().toFloat();
    map.center=vec_line_edit[3]->text().toInt();

    car.scale.x=vec_line_edit[4]->text().toFloat();
    car.scale.y=vec_line_edit[5]->text().toFloat();

    goal_point.x=vec_line_edit[6]->text().toFloat();
    goal_point.y=vec_line_edit[7]->text().toFloat();

    countBarriers=spinBoxN->value();

    vec_buf_barrier=vec_barrier;
    goal_point_buf=goal_point;
}

void MainWindow::recalculateBarrier(Point p)
{
    for(unsigned int i=0; i<vec_buf_barrier.size();i++)
    {
        vec_buf_barrier[i].position.x=vec_buf_barrier[i].position.x-p.x;
        vec_buf_barrier[i].position.y=vec_buf_barrier[i].position.y-p.y;
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
    dataStream<<map<<car;
    dataStream<<goal_point_buf;
    dataStream<<spinBoxN->value();
    for(unsigned int i=0; i<vec_buf_barrier.size();i++)
        dataStream<<vec_buf_barrier[i];
    socket->sendData(arr);
}

void MainWindow::on_sendDataBtn_clicked()
{
    updateValues();
    stopSimulation=false;
    socket->auto_mode(isMotionSimulation);
    socket->wnd=this;
    //если нет симуляции движения
    if(!isMotionSimulation)
    {
        makePack();
    }
    //если есть симуляция движения
    else
    {
        disconnect(socket->GetQTcpSocket(),SIGNAL(readyRead()),socket,SLOT(readyReadNoSimulation()));
        connect(socket->GetQTcpSocket(),SIGNAL(readyRead()),socket,SLOT(readyReadSimpleSimulation()));
        if(isEndlessSimulation==true)
        {
            disconnect(socket->GetQTcpSocket(),SIGNAL(readyRead()),socket,SLOT(readyReadSimpleSimulation()));
            connect(socket->GetQTcpSocket(),SIGNAL(readyRead()),socket,SLOT(readyReadEndlessSimulation()));
        }
        socket->SetDelay_time(vec_line_edit[vec_line_edit.size()-1]->text().toInt());
        //если не ожидаем БПР
        if(!isWaitBPR)
        {
            countReceivePack=0;
            countSendPack=0;
            stop_btn->setEnabled(true);
            ui->sendDataBtn->setEnabled(false);
            if(vec_barrier.size()==0)
            {
                count_line=goal_point.y+1;
                count_line_buf=count_line;
            }
            else
            {

                int max=-100;
                int min=+100;
                for(unsigned int i=0; i<vec_barrier.size(); i++)
                {
                    Point left_top(vec_barrier[i].position.x-vec_barrier[i].scale.x/2,vec_barrier[i].position.y+vec_barrier[i].scale.y/2);
                    Point right_bottom(vec_barrier[i].position.x+vec_barrier[i].scale.x/2,vec_barrier[i].position.y-vec_barrier[i].scale.y/2);

                    if (abs(left_top.x) > abs(int(left_top.x / map.step) * map.step))
                        left_top.x -= abs(left_top.x - int(left_top.x / map.step) * map.step);

                    if (abs(right_bottom.x) > abs(int(right_bottom.x / map.step) * map.step))
                        right_bottom.x +=abs(right_bottom.x - int(right_bottom.x / map.step) * map.step);

                    if (abs(left_top.y) > abs(int(left_top.y / map.step) * map.step))
                        left_top.y +=abs(left_top.y - int(left_top.y / map.step) * map.step);

                    if (abs(right_bottom.y) > abs(int(right_bottom.y / map.step) * map.step))
                        right_bottom.y -= abs(right_bottom.y - int(right_bottom.y / map.step) * map.step);

                    if(int(left_top.y)/map.step>max) max=int(left_top.y);
                    if(int(right_bottom.y)/map.step<min) min=int(right_bottom.y);
                }
                count_line=max+2;
                count_line_buf=count_line;
            }
            while(!stopSimulation)
            {
                makePack();
            }
        }
        else
        {
            //если движение зациклено или точка маршрута недостижима
            if(isLoopSimulation||isEndlessSimulation)
            {
                stop_btn->setEnabled(true);
                ui->sendDataBtn->setEnabled(false);
            }
            //если движение зациклено и точка маршрута недостижима
            if(isLoopSimulation&&isEndlessSimulation)
            {
                if(vec_barrier.size()==0)
                {
                    count_line=goal_point.y+1;
                    count_line_buf=count_line;
                }
                else
                {
                    int max=-100;
                    int min=+100;
                    for(unsigned int i=0; i<vec_barrier.size(); i++)
                    {
                        Point left_top(vec_barrier[i].position.x-vec_barrier[i].scale.x/2,vec_barrier[i].position.y+vec_barrier[i].scale.y/2);
                        Point right_bottom(vec_barrier[i].position.x+vec_barrier[i].scale.x/2,vec_barrier[i].position.y-vec_barrier[i].scale.y/2);

                        if (abs(left_top.x) > abs(int(left_top.x / map.step) * map.step))
                            left_top.x -= abs(left_top.x - int(left_top.x / map.step) * map.step);

                        if (abs(right_bottom.x) > abs(int(right_bottom.x / map.step) * map.step))
                            right_bottom.x +=abs(right_bottom.x - int(right_bottom.x / map.step) * map.step);

                        if (abs(left_top.y) > abs(int(left_top.y / map.step) * map.step))
                            left_top.y +=abs(left_top.y - int(left_top.y / map.step) * map.step);

                        if (abs(right_bottom.y) > abs(int(right_bottom.y / map.step) * map.step))
                            right_bottom.y -= abs(right_bottom.y - int(right_bottom.y / map.step) * map.step);

                        if(int(left_top.y)/map.step>max) max=int(left_top.y);
                        if(int(right_bottom.y)/map.step<min) min=int(right_bottom.y);
                    }
                    count_line=max+2;
                    count_line_buf=count_line;
                }
            }
            makePack();
        }
    }
    if(isSmoothing)
    {
        window->setBarriers(vec_buf_barrier);
        window->setCar(car);
        window->show();
    }
}


void MainWindow::on_SettingsBtn_triggered()
{

}

void MainWindow::on_importSeq_triggered()
{
    QString str = QFileDialog::getOpenFileName(0, "Open Dialog", "sequences", "*.xml");
    countBarriers=0;
    XMLGenerator::importXML(str,map,car,goal_point,vec_barrier, countBarriers);
    updateWidgets();
}


void MainWindow::on_exportSeq_triggered()
{
    QString str = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),"sequences",tr ("*.xml" ));
    updateValues();
    XMLGenerator::exportXML(str,map,car,goal_point,vec_barrier);
}
