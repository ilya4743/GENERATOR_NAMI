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
    void doConnect();

signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void readyRead1();

private:
    QTcpSocket *socket;

public:
    MainWindow* wnd;
    bool auto_send;

    void auto_mode(bool isAuto);
};

#endif // MYTCPSOCKET_H
