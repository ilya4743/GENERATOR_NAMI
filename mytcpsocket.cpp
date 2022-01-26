#include "mytcpsocket.h"
#include<QDataStream>
#include <QTimer>
#include<QTime>
#include<QCoreApplication>

MyTcpSocket::MyTcpSocket(QObject *parent) : QObject(parent)
{
}

void MyTcpSocket::doConnect()
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

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

void MyTcpSocket::readyRead()
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

void delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

void MyTcpSocket::readyRead1()
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
            wnd->recalculateData(Point (x,y));
            wnd->makePack();
        }
    }
    socket->readAll();
}

void MyTcpSocket::auto_mode(bool isAuto)
{
    auto_send=isAuto;
    if(auto_send)
    {
        disconnect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
        connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead1()));
    }
    else
    {
        disconnect(socket, SIGNAL(readyRead()),this, SLOT(readyRead1()));
        connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));
    }
}


void MyTcpSocket::sendData(QByteArray& arr)
{
    delay();
    socket->write(arr);
    socket->flush();
}
