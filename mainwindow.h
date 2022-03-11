#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>
#include <vector>
#include <QCheckBox>
#include "mytcpserver.h"
#include "mytcpsocket.h"
#include"point.h"
#include <QPushButton>
#include"settings.h"
#include<QSpinBox>
#include<QXmlStreamWriter>

using namespace std;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
class Settings;
QT_END_NAMESPACE
class Barrier
{
public:
    float x;
    float y;
    float width;
    float height;
    Barrier():x(0),y(0),width(0), height(0){}
    Barrier(float x,float y,float width,float height):x(x),y(y),width(width),height(height){}
    Barrier(const Barrier& barrier):x(barrier.x),y(barrier.y), width(barrier.width), height(barrier.height){}
    ~Barrier(){};
};

class BarrierXMLWriter
{
public:
    int countBarrier=0;
    void print(QXmlStreamWriter& XMLWriter, const Barrier& barrier)
    {
        XMLWriter.writeStartElement("node");
        XMLWriter.writeAttribute("name", "Cube." + QString::number(countBarrier));

        XMLWriter.writeStartElement("position");
        XMLWriter.writeAttribute("x",QString::number(barrier.x));
        XMLWriter.writeAttribute("y",QString::number(barrier.y));
        XMLWriter.writeAttribute("z","0" );
        XMLWriter.writeEndElement();

        XMLWriter.writeStartElement("rotation");
        XMLWriter.writeAttribute("qw","0");
        XMLWriter.writeAttribute("qx","0");
        XMLWriter.writeAttribute("qy","0");
        XMLWriter.writeAttribute("qz","0" );
        XMLWriter.writeEndElement();

        XMLWriter.writeStartElement("scale");
        XMLWriter.writeAttribute("x",QString::number(barrier.width));
        XMLWriter.writeAttribute("y",QString::number(barrier.height));
        XMLWriter.writeAttribute("z","1" );
        XMLWriter.writeEndElement();

        XMLWriter.writeStartElement("entity");
        XMLWriter.writeAttribute("meshFile","Cube."+ QString::number(countBarrier)+".mesh");
        XMLWriter.writeAttribute("name","Cube." + QString::number(countBarrier));
        XMLWriter.writeStartElement("userData");
        XMLWriter.writeEndElement();
        XMLWriter.writeEndElement();
        XMLWriter.writeEndElement();

        ++countBarrier;
    }
};

class MyTcpSocket;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
signals:
    void data_received(QDataStream &stream);
private slots:

    void on_ExitBtn_triggered();

    void textChanged(const QString &arg1);
    void textChanged1(const QString &s);
    void textChangedBarrier1(const QString &arg1);
    void textChangedBarrier2(const QString &arg1);
    void textChangedBarrier3(const QString &arg1);
    void textChangedBarrier4(const QString &arg1);

    void currentIndexCenterChanged(int index);
    void on_comboBox_currentIndexChanged(int index);


    void on_action_triggered();
    void motionSimulation(int state);
    void loopSimulation(int state);
    void endlessSimulation(int state);
    void waitBPR(int state);
    void on_sendDataBtn_clicked();
    void stopSimulationBtn_click();
    void on_SettingsBtn_triggered();

    void on_action_2_triggered();

private:
    int ind;
    vector<QLineEdit*> vec_line_edit;
    vector<QLabel*> vec_label;
    MyTcpServer* server;
    MyTcpSocket* socket;
    QComboBox qComboBox;
    Settings *settings_wnd;
    vector<QCheckBox*> vec_check_box;
    QSpinBox* spinBoxN;

public:
    QPushButton *stop_btn;
    Ui::MainWindow *ui;
    vector<Barrier> vec_barrier;
    vector<Barrier> vec_buf_barrier;
    Point goal_point;
    Point goal_point_buf;
    int count_line;
    int count_line_buf;
    int countSendPack;
    int countReceivePack;
    void recalculateBarrier(Point P);
    void recalculateGoal(Point p);
    void recalculateGoalX(float x);
    void makePack();
    bool isMotionSimulation;
    bool isLoopSimulation;
    bool isEndlessSimulation;
    bool isWaitBPR;
    bool stopSimulation;
};
#endif // MAINWINDOW_H
