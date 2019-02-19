#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <string>
#include <ctime>
#include <mutex>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = 0);
    ~TCPClient();
    bool connect(std::string host, int port);
    bool disConnect();
    bool isConnected();
    bool writeCount(const char* data, const int length);

signals:
    void ResponseReady(QByteArray);
public slots:
    void readDownlinkData();
    void ReadError(QAbstractSocket::SocketError error);
private:
    QTcpSocket* tcpsocket;
};

#endif // TCPCLIENT_H
