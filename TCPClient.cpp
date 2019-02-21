#include "TCPClient.h"

TCPClient::TCPClient(QObject *parent) : QObject(parent)
{
    tcpsocket = NULL;
    m_tcpClientThread = NULL;
    m_isReadyRead = false;

    createTCPSocketAndThread();
}

TCPClient::~TCPClient()
{
    destroyTCPSocketAndThread();
}

bool TCPClient::createTCPSocketAndThread()
{
    if(tcpsocket == NULL)
    {
        tcpsocket = new QTcpSocket();
        if(tcpsocket == NULL)
        {
            qDebug() << "create QTcpSocket error";
            return false;
        }
    }
    if(m_tcpClientThread == NULL)
    {
        m_tcpClientThread = new (std::nothrow) QThread();
        if(m_tcpClientThread == NULL)
        {
            qDebug() << "create QThread for QTcpSocket error";
            delete tcpsocket;
            tcpsocket = NULL;
            return false;
        }
    }

    this->moveToThread(m_tcpClientThread);

    QObject::connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readDownlinkData()));
    QObject::connect(tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)),
                     this, SLOT(ReadError(QAbstractSocket::SocketError)));

    m_tcpClientThread->start();
    return true;
}

void TCPClient::destroyTCPSocketAndThread()
{
    QObject::disconnect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readDownlinkData()));
    QObject::disconnect(tcpsocket, SIGNAL(error(QAbstractSocket::SocketError)),
                        this, SLOT(ReadError(QAbstractSocket::SocketError)));

    if(tcpsocket)
    {
        delete tcpsocket;
        tcpsocket = NULL;
    }
    if(m_tcpClientThread)
    {
        delete m_tcpClientThread;
        m_tcpClientThread = NULL;
    }
}

bool TCPClient::connect(const std::string host, const int port)
{
    if(tcpsocket == NULL)
    {
        qDebug() << "tcpsocket == NULL";
        return false;
    }

    if (tcpsocket->state() == QAbstractSocket::ConnectedState)
    {
        return true;
    }

    tcpsocket->connectToHost(QString(host.c_str()), port);
    if(false == tcpsocket->waitForConnected(TCP_WAIT_FOR_CONNECT_TIMEOUT_MSEC))
    {
        destroyTCPSocketAndThread();
        return false;
    }
    return true;
}

bool TCPClient::disConnect()
{
    if(tcpsocket && m_tcpClientThread)
    {
        tcpsocket->disconnectFromHost();
        if (tcpsocket->state() != QAbstractSocket::UnconnectedState)
        {
            tcpsocket->waitForDisconnected(1000);
        }
    }
    destroyTCPSocketAndThread();
    return true;
}

bool TCPClient::isConnected()
{
    if(tcpsocket == NULL)
    {
        qDebug() << "tcpsocket == NULL";
        return false;
    }
    return tcpsocket->state() == QAbstractSocket::ConnectedState;
}

void TCPClient::readDownlinkData()
{
    qDebug() << "read downlink data ready";
    m_isReadyReadMutex.lock();
    m_isReadyRead = true;
    m_isReadyReadMutex.unlock();
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
        qDebug() << "tcpsocket == NULL";
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

QByteArray TCPClient::readCount(int p_timeout)
{
    if(tcpsocket == NULL)
    {
        qDebug() << "tcpsocket == NULL";
        return QByteArray();
    }

    std::time_t l_startTimestamps = std::time(NULL);
    do
    {
        m_isReadyReadMutex.lock();
        if(m_isReadyRead == true)
        {
            QByteArray buffer = tcpsocket->readAll();
            if(buffer.isEmpty() == false)
            {
                m_isReadyRead = false;
                m_isReadyReadMutex.unlock();
                return buffer;
            }
        }
        m_isReadyReadMutex.unlock();
        Utility::Msleep(100);
    }
    while(std::time(NULL) <= (l_startTimestamps + p_timeout));
    qDebug() << "TCPClient::readCount timeout";
    return QByteArray();
}
