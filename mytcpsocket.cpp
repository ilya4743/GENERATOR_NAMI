#include "mytcpsocket.h"
#include<QDataStream>
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
    /*float width=18.1;

    float height=5.1;
    float step=0.1;

    int num_w=width/step;
    int num_h=height/step;
//(num_w)/2+(num_h/2)*(num_w);
    int center=(num_w)/2+(num_h-1)*(num_w);

    QByteArray arr;
    QDataStream data(&arr, QIODevice::WriteOnly);
    //d.setVersion(QDataStream::Qt_5_3);

    data<<float(width)<<float(height)<<float(step);
    data<<int(center);

    //размеры машины
    data<<float(0)<<float(0);

    //куда едем
    data<<float(+9)<<float((2.5));
    //кол-во препятствий
    data<<int(0);

    //препятствия

    //data<<float(1.9)<<float(0.75-0.25/2-0.75)<<float(0.6)<<float(0.25+1.3);
    //data<<float(0.0)<<float(1-0.2)<<float(10)<<float(0.1);
    //data<<float(0.0)<<float(-1.0)<<float(3)<<float(0.1);

    //data<<float(-0.3)<<float(-0.1)<<float(0)<<float(0);
    //data<<float(1)<<float(1)<<float(0.21)<<float(0.1);

    //высота покрытия сетки графа
    socket->write(arr);
    //socket->flush();*/

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
    float x,y;
    int error;
    in>>error;
    if(error==0)
        while(socket->bytesAvailable())
        {
            in>>x>>y;
            qDebug()<<x<<'\t'<<y;
        }
    else
    qDebug()<<"Error "<<error;

    //socket->disconnectFromHost();
}

void MyTcpSocket::sendData(QByteArray& arr)
{
    /*QByteArray arr;
    QDataStream dataStream(&arr, QIODevice::WriteOnly);
    dataStream.setFloatingPointPrecision(QDataStream::SinglePrecision);
    dataStream.setByteOrder(QDataStream::LittleEndian);
    dataStream<<(unsigned char)0x44<<(unsigned char)0x47;

    for(int i=0; i<3;i++)
        dataStream<<data[i];
    dataStream<<int(data[3]);

    for(int i=4; i<data.size()-1; i++)
    dataStream<<data[data.size()-1];*/
    socket->write(arr);
    socket->flush();
}
