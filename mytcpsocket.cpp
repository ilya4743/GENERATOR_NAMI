#include "mytcpsocket.h"
#include<QDataStream>
#include <QTimer>
#include<QTime>
#include<QCoreApplication>

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
    delay_time=1000;
}

void MyTcpSocket::doConnect(const QString& IP, const int PORT, const int RECONNECT_TIME)
{
    this->IP=IP;
    this->PORT=PORT;
    reconnect_time=RECONNECT_TIME;
    qDebug() << "connecting...";
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyReadNoSimulation()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError )),this,SLOT(Error(QAbstractSocket::SocketError)));
    socket->connectToHost(IP, PORT);
}

void MyTcpSocket::connected()
{
    qDebug() << "connected...";
}

void MyTcpSocket::disconnected()
{
    qDebug() << "disconnected...";
}

void MyTcpSocket::reconnect()
{
    socket->connectToHost(IP, PORT);
}

void MyTcpSocket::Error(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Error: " << socketError;
    qDebug()<<"Reconnect!";
    QTimer::singleShot(reconnect_time, this, SLOT(reconnect()));
}

void MyTcpSocket::bytesWritten(qint64 bytes)
{
    //qDebug() << bytes << " bytes written...";
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
            //qDebug()<<x<<'\t'<<y;
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
        //qDebug()<<x<<y;
        wnd->recalculateBarrier(Point (x,y));
        wnd->recalculateGoalX(x);
        wnd->count_line_buf=wnd->count_line_buf-y;
        //если зациклено, нужно ли переводить на новую итерацию
        if (wnd->isLoopSimulation&&wnd->count_line_buf==0)
        {
            wnd->vec_buf_barrier=wnd->vec_barrier;
            wnd->goal_point_buf=wnd->goal_point;
            wnd->count_line_buf=wnd->count_line;
        }
        if(wnd->isWaitBPR)
        {
            delay(delay_time);
            wnd->makePack();
        }
        else
            wnd->countReceivePack++;
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
        wnd->countReceivePack++;
        int n;
        in>>n;
        in>>x>>y;
        //qDebug()<<x<<y;
        if(!(x==0&&y==0))
        {
            wnd->recalculateBarrier(Point (x,y));
            wnd->recalculateGoal(Point (x,y));
            if(wnd->isWaitBPR)
            {
                delay(delay_time);
                wnd->makePack();
            }
            //else
                //wnd->countReceivePack++;
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
            //else
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
    wnd->countSendPack++;
    socket->write(arr);
    socket->flush();
    if(!wnd->isWaitBPR)
        delay(delay_time);
    //QTimer::singleShot(delay_time, this);

}
