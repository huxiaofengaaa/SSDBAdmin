#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDebug>
#include <string>
#include <ctime>
#include <mutex>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(const bool isSync, QObject *parent = 0);
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
    QTcpSocket* tcpsocket;
    const bool m_type;
};

#endif // TCPCLIENT_H
