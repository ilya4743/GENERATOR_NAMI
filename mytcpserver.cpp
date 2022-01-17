#include "mytcpserver.h"
#include<QProcess>
#include<iostream>

MyTcpServer::MyTcpServer(QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);

    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()), this, SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any, 15555))
    {
        qDebug() << "Server could not start";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void MyTcpServer::newConnection()
{
    socket = server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(readyRead()));

    qDebug() << "connected...";

    /*
    float width=11;
    float height=10;
    float step=1;

    int num_w=width/step;
    int num_h=height/step;
//(num_w)/2+(num_h/2)*(num_w);
    int center=(num_w)/2+(num_h-1)*(num_w);

    QByteArray arr;
    QDataStream data(&arr, QIODevice::WriteOnly);
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);
    data.setByteOrder(QDataStream::LittleEndian);
    data<<(unsigned char)0x44<<(unsigned char)0x47;
    data<<float(width)<<float(height)<<float(step);
    data<<int(center);

    //размеры машины
    data<<float(0)<<float(0);

    //куда едем
    data<<float(-4)<<float((4));

    //кол-во препятствий
    data<<int(1);

    //препятствия
    data<<float(2)<<float(4)<<float(3)<<float(1);

    //высота покрытия сетки графа
    socket->write(arr);
    socket->flush();*/
}

void MyTcpServer::readyRead()
{
    QDataStream in(socket);
    in.setFloatingPointPrecision(QDataStream::SinglePrecision);
    in.setByteOrder(QDataStream::LittleEndian);
    unsigned char c1, c2;
    in>>c1>>c2;

    if(c1==0x44 && c2==0x48)
    {
        int n;
        in>>n;
        std::cout<<n<<std::endl;
        for(int i=0; i<n; i++)
        {
           float x, y;
           in>>x>>y;
           std::cout<<x<<'\t'<<y<<std::endl;
        }
    }
    socket->readAll();

/*
    float width=11;
    float height=10;
    float step=1;

    int num_w=width/step;
    int num_h=height/step;
//(num_w)/2+(num_h/2)*(num_w);
    int center=(num_w)/2+(num_h-1)*(num_w);

    QByteArray arr;
    QDataStream data(&arr, QIODevice::WriteOnly);
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);
    data.setByteOrder(QDataStream::LittleEndian);
    data<<(unsigned char)0x44<<(unsigned char)0x47;
    data<<float(width)<<float(height)<<float(step);
    data<<int(center);

    //размеры машины
    data<<float(0)<<float(0);

    //куда едем
    data<<float(-4)<<float((4));

    //кол-во препятствий
    data<<int(1);

    //препятствия
    data<<float(-2)<<float(4)<<float(3)<<float(1);
    //высота покрытия сетки графа
    socket->write(arr);
    socket->flush();*/
    //socket->disconnectFromHost();
}

void MyTcpServer::sendData(std::vector<float>& data)
{
    QByteArray arr;
    QDataStream dataStream(&arr, QIODevice::WriteOnly);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream<<(unsigned char)0x44<<(unsigned char)0x47;

    for(int i=0; i<3;i++)
        dataStream<<data[i];
    dataStream<<int(data[3]);

    for(int i=4; i<data.size()-1; i++)
    dataStream<<data[data.size()-1];
    socket->write(arr);
    socket->flush();
}
