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
    friend QDataStream& operator <<(QDataStream &out, const Barrier &b);
    friend QDataStream& operator >>(QDataStream &in, Barrier &b);
};

inline QDataStream& operator <<(QDataStream &out, const Barrier &b)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << b.x;
    out << b.y;
    out << b.width;
    out << b.height;
    return out;
}

inline QDataStream& operator >>(QDataStream &in, Barrier &b)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> b.x;
    in >> b.y;
    in >> b.width;
    in >> b.height;
    return in;
}

class Car
{
private:

public:
    float width;
    float height;
    Car():width(0),height(0){}
    Car(float width, float height):width(width),height(height){}
    Car(const Car& car):width(car.width), height(car.height){}
    ~Car(){}
    friend QDataStream& operator <<(QDataStream &out, const Car &car);
    friend QDataStream& operator >>(QDataStream &in, Car &car);
};

inline QDataStream& operator <<(QDataStream &out, const Car &car)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << car.width;
    out << car.height;
    return out;
}

inline QDataStream& operator >>(QDataStream &in, Car &car)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> car.width;
    in >> car.height;
    return in;
}

class CarXMLWriter
{
private:
public:
    void print(QXmlStreamWriter& XMLWriter, const Car& car)
    {
        XMLWriter.writeStartElement("Car");
        XMLWriter.writeAttribute("width",QString::number(car.width));
        XMLWriter.writeAttribute("height",QString::number(car.height));
        XMLWriter.writeEndElement();
    }
};

class GameMap
{
private:

public:
    float width;
    float height;
    float step;
    int center;
    GameMap():width(0),height(0),step(0),center(0){}
    GameMap(float width, float height,float step,float center):width(width),height(height),step(step),center(center){}
    GameMap(const GameMap& map):width(map.width),height(map.height),step(map.step),center(map.center){}
    ~GameMap(){}
    friend QDataStream& operator <<(QDataStream &out, const GameMap &map);
    friend QDataStream& operator >>(QDataStream &in, GameMap &map);
};

inline QDataStream& operator <<(QDataStream &out, const GameMap &map)
{
    out.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    out.setByteOrder(QDataStream::LittleEndian);
    out << map.width;
    out << map.height;
    out << map.step;
    out << map.center;
    return out;
}

inline QDataStream& operator >>(QDataStream &in, GameMap &map)
{
    in.setFloatingPointPrecision(QDataStream::FloatingPointPrecision());
    in.setByteOrder(QDataStream::LittleEndian);
    in >> map.width;
    in >> map.height;
    in >> map.step;
    in >> map.center;
    return in;
}

class GameMapXMLWriter
{
private:

public:
    void print(QXmlStreamWriter& XMLWriter, const GameMap& map)
    {
        XMLWriter.writeStartElement("GameMap");
        XMLWriter.writeAttribute("width",QString::number(map.width));
        XMLWriter.writeAttribute("height",QString::number(map.height));
        XMLWriter.writeAttribute("step",QString::number(map.step));
        XMLWriter.writeAttribute("center",QString::number(map.center));
        XMLWriter.writeEndElement();
    }
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

#include <QXmlAttributes>
#include <QXmlDefaultHandler>
//#include <QXmlInputSource>

class BarrierParser : public QXmlDefaultHandler
{
private:
    QString m_strText;
    bool isCorrect=false;
    QDataStream *dataStream;
public:
    QByteArray data;
    BarrierParser()
    {
        dataStream=new QDataStream(&data,QIODevice::WriteOnly);
        dataStream->setFloatingPointPrecision(QDataStream::SinglePrecision);
        dataStream->setByteOrder(QDataStream::LittleEndian);
    }

    ~BarrierParser()
    {
        delete dataStream;
    }

    bool startElement (const QString& namespaceURI, const QString& localName, const QString& name, const QXmlAttributes& attrs)
    {
            if(name!="scene" && name!="node" && name!="rotation" && name!="entity" && name!="userData" && name !="externals" && name!="environment" && name!="colourBackground")
            {
                if(name=="position"&&isCorrect==false)
                {
                    if(attrs.qName(0)=='x' && attrs.qName(1)=='y' && attrs.qName(2)=='z')
                    {                        
                        *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                        qDebug()<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                        isCorrect=true;
                        return true;
                    }
                }
                if( name=="scale"&&isCorrect==true)
                {
                    *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                    qDebug()<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat();
                    isCorrect=false;
                    return true;
                }
                if(name=="GameMap")
                {
                    *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat()<<attrs.value(2).toFloat()<<attrs.value(3).toInt();
                    return true;
                }
                if(name=="Car")
                {
                    *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat();
                    return true;
                }
                if(name=="CountBarriers")
                {
                    *dataStream<<attrs.value(0).toInt();
                    return true;
                }
                if(name=="Goal")
                {
                    *dataStream<<attrs.value(0).toFloat()<<attrs.value(1).toFloat();
                    return true;
                }
                return false;
            }
            return true;
    }

    bool characters(const QString& strText)
    {
        m_strText=strText;
        return true;
    }

    bool endElement(const QString& namespaceURI, const QString& localName, const QString& qName)
    {
        if(qName!="contact"&&qName!="addressbook")
        {
            qDebug()<<"TagName:"<<qName<<"\tText:"<<m_strText;
        }
        return true;
    }

    bool fatalError (const QXmlParseException& exception)
    {
        qDebug()<<"Line: "<<exception.lineNumber()<<", Column:"<<exception.columnNumber()<<", Message:"<<exception.message();
        return false;
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
    void updateWidgets();
    void updateValues();
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
    Car car;
    GameMap map;
    int countBarriers;
};
#endif // MAINWINDOW_H
