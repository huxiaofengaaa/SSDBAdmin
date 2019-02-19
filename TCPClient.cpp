#include "TCPClient.h"

TCPClient::TCPClient(QObject *parent) : QObject(parent)
{
    tcpsocket = new QTcpSocket();
}

TCPClient::~TCPClient()
{
    if(tcpsocket)
    {
        delete tcpsocket;
        tcpsocket = NULL;
    }
}

bool TCPClient::connect(std::string host, int port)
{
    if(tcpsocket == NULL)
    {
        return false;
    }
    if (tcpsocket->state() == QAbstractSocket::ConnectedState)
    {
        return true;
    }
    else
    {
        tcpsocket->connectToHost(QString(host.c_str()), port);
        if(false == tcpsocket->waitForConnected(1000))
        {
            delete tcpsocket;
            tcpsocket = NULL;
            return false;
        }
        else
        {
            QObject::connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readDownlinkData()));
            QObject::connect(tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)),this,
                             SLOT(ReadError(QAbstractSocket::SocketError)));
            return true;
        }
    }
}

bool TCPClient::disConnect()
{
    if(tcpsocket)
    {
        tcpsocket->disconnectFromHost();
        if (tcpsocket->state() != QAbstractSocket::UnconnectedState)
        {
            tcpsocket->waitForDisconnected(1000);
        }
        QObject::disconnect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readDownlinkData()));
        QObject::disconnect(tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)), this,
                            SLOT(ReadError(QAbstractSocket::SocketError)));
        delete tcpsocket;
        tcpsocket = NULL;
    }
    return true;
}

bool TCPClient::isConnected()
{
    if(tcpsocket == NULL)
    {
        return false;
    }
    return tcpsocket->state() == QAbstractSocket::ConnectedState;
}

void TCPClient::readDownlinkData()
{
    QByteArray buffer = tcpsocket->readAll();
    if(false == buffer.isEmpty())
    {
        emit ResponseReady(buffer);
    }
}

void TCPClient::ReadError(QAbstractSocket::SocketError error)
{
    qDebug() << "ReadError " << error;
    disConnect();
}

bool TCPClient::writeCount(const char* data, const int length)
{
    if(tcpsocket == NULL)
    {
        return false;
    }

    int alreadyWrite = 0;
    while(alreadyWrite < length)
    {
        int nwrite = tcpsocket->write(data + alreadyWrite, length - alreadyWrite);
        if(nwrite < 0)
        {
            break;
        }
        alreadyWrite += nwrite;
    }
    return alreadyWrite == length;
}
