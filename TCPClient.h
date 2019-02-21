#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <QThread>
#include <string>
#include <ctime>
#include <mutex>
#include "Utility.h"

#define TCP_WAIT_FOR_CONNECT_TIMEOUT_MSEC        1000

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = 0);
    ~TCPClient();
    bool connect(const std::string host, const int port);
    bool disConnect();
    bool isConnected();
    bool writeCount(const char* data, const int length);
    QByteArray readCount(int p_timeout);

signals:
    void ResponseReady(QByteArray);
public slots:
    void readDownlinkData();
    void ReadError(QAbstractSocket::SocketError error);
private:
    bool createTCPSocketAndThread();
    void destroyTCPSocketAndThread();

    QTcpSocket* tcpsocket;
    QThread*    m_tcpClientThread;
    bool        m_isReadyRead;
    std::mutex  m_isReadyReadMutex;
};

#endif // TCPCLIENT_H
