#include "mytcpsocket.h"
#include<QDataStream>
#include <QTimer>
#include<QTime>
#include<QCoreApplication>

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
    delay_time=1000;
}

void MyTcpSocket::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyReadNoSimulation()));

    qDebug() << "connecting...";

    // this is not blocking call
    socket->connectToHost("localhost", 15555);

    // we need to wait...
    if(!socket->waitForConnected(5000))
    {
        qDebug() << "Error: " << socket->errorString();
    }
}

void MyTcpSocket::connected()
{
    qDebug() << "connected...";
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    qDebug() << bytes << " bytes written...";
}

void MyTcpSocket::readyReadNoSimulation()
{
    QDataStream in(socket);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setByteOrder(QDataStream::LittleEndian);
    float x,y;
    unsigned char b1, b2;
    in>>b1>>b2;
    int n;
    in>>n;
    if(b1==0x44&&b2==0x48)
        while(socket->bytesAvailable())
        {
            in>>x>>y;
            qDebug()<<x<<'\t'<<y;
        }
}

void delay(const int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MyTcpSocket::readyReadEndlessSimulation()
{
    QDataStream in(socket);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setByteOrder(QDataStream::LittleEndian);
    float x=-100,y=-100;
    unsigned char b1, b2;
    in>>b1>>b2;

    if(b1==0x44&&b2==0x48)
    {
        int n;
        in>>n;
        in>>x>>y;
        qDebug()<<x<<y;
        wnd->recalculateBarrier(Point (x,y));
        wnd->recalculateGoalX(x);
        wnd->count_line=wnd->count_line-y;
        //если зациклено, нужно ли переводить на новую итерацию
        if (wnd->isLoopSimulation&&wnd->count_line==0)
        {
            wnd->vec_buf_barrier=wnd->vec_barrier;
            wnd->goal_point_buf=wnd->goal_point;
            wnd->count_line=40;
        }
        if(wnd->isWaitBPR)
        {
            delay(delay_time);
            wnd->makePack();
        }
    }
    socket->readAll();
}

void MyTcpSocket::readyReadSimpleSimulation()
{
    QDataStream in(socket);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setByteOrder(QDataStream::LittleEndian);
    float x=-100,y=-100;
    unsigned char b1, b2;
    in>>b1>>b2;

    if(b1==0x44&&b2==0x48)
    {
        int n;
        in>>n;
        in>>x>>y;
        qDebug()<<x<<y;
        if(!(x==0&&y==0))
        {
            wnd->recalculateBarrier(Point (x,y));
            wnd->recalculateGoal(Point (x,y));
            if(wnd->isWaitBPR)
            {
                delay(delay_time);
                wnd->makePack();
            }
        }
        //если зациклено, нужно ли переводить на новую итерацию
        else if(wnd->isLoopSimulation)
        {
            wnd->vec_buf_barrier=wnd->vec_barrier;
            wnd->goal_point_buf=wnd->goal_point;
            if(wnd->isWaitBPR)
            {
                delay(delay_time);
                wnd->makePack();
            }
        }

    }
    socket->readAll();
}

void MyTcpSocket::auto_mode(bool isAuto)
{
    auto_send=isAuto;
}

void MyTcpSocket::sendData(QByteArray& arr)
{
    //if(wnd->isMotionSimulation)
    socket->write(arr);
    socket->flush();
    QString str=QTime::currentTime().toString("HH:mm:ss");
    qDebug()<<str;
    if(!wnd->isWaitBPR)
        delay(delay_time);
}
