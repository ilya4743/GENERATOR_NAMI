#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QDebug>
#include <QTimer>
#include "mainwindow.h"
class MainWindow;
class MyTcpSocket : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpSocket(QObject *parent = 0);
    void sendData(QByteArray& arr);
    void doConnect(const QString& IP, const int PORT, const int RECONNECT_TIME);
    void changeModeBPR(bool isSmoothing);
signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyReadSimpleSimulation();
    void readyReadEndlessSimulation();
    void readyReadNoSimulation();
    void reconnect();
    void Error(QAbstractSocket::SocketError socketError);

private:
    QTcpSocket *socket;
    int delay_time;
    int reconnect_time;
    QString IP;
    int PORT;
public:
    MainWindow* wnd;
    bool auto_send;
    int GetDelay_time(){return delay_time;}
    void SetDelay_time(int delay_time){this->delay_time=delay_time;}
    void auto_mode(bool isAuto);
    QTcpSocket* GetQTcpSocket(){return socket;}
};

#endif // MYTCPSOCKET_H
