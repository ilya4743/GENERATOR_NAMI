#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H
#include <QTcpSocket>
#include <QDataStream>
#include <QObject>
#include <QTcpServer>
#include <vector>

class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QObject *parent = nullptr);
    void sendData(std::vector<float>& data);

signals:

public slots:
    void newConnection();
    void readyRead();
private:
    void sendToClient(QTcpSocket* socket, QDataStream& data);
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MYTCPSERVER_H
