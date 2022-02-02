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

void delay(const int ms)
{
    QTime dieTime= QTime::currentTime().addMSecs(ms);
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
            //если конечная точка маршрута достижима
            if(wnd->vec_check_box[2]->checkState()==Qt::CheckState::Unchecked)
            {
                wnd->recalculateBarrier(Point (x,y));
                wnd->recalculateGoal(Point (x,y));
            }
            //если конечная точка маршрута недостижима
            else if(wnd->vec_check_box[2]->checkState()==Qt::CheckState::Checked)
            {
                wnd->recalculateBarrier(Point (x,y));
                wnd->recalculateGoalX(x);
                //wnd->recalculateGoal(Point (x,y));

                wnd->count_line=wnd->count_line-y;
                //если зациклено, нужно ли переводить на новую итерацию
                if (wnd->vec_check_box[1]->checkState()==Qt::CheckState::Checked&&wnd->count_line==0)
                {
                    wnd->vec_buf_barrier=wnd->vec_barrier;
                    wnd->goal_point_buf=wnd->goal_point;
                    wnd->count_line=40;
                }
            }
            //если ожидаем БПР
            if(wnd->isWaitBPR)
                wnd->makePack();
        }
        //на новую итерацию, если достигли конца маршрута при симуляции
        else if(wnd->vec_check_box[1]->checkState()==Qt::CheckState::Checked)
        {
            wnd->vec_buf_barrier=wnd->vec_barrier;
            wnd->goal_point_buf=wnd->goal_point;
            //если ожидаем БПР
            if(wnd->isWaitBPR)
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

    delay(delay_time);
    socket->write(arr);
    socket->flush();
}
