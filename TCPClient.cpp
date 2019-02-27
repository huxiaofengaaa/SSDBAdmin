#include "TCPClient.h"

TCPClient::TCPClient(const std::string host, const int port):
    m_resolver(asio::ip::tcp::resolver(m_ioContext)),
    m_tcpSocket(asio::ip::tcp::socket(m_ioContext))
{
    m_host = host;
    m_port = port;
}

TCPClient::~TCPClient()
{

}

bool TCPClient::connect(const std::string host, const int port)
{
    m_host = host;
    m_port = port;

    asio::ip::tcp::resolver::results_type l_endpointResult = m_resolver.resolve(m_host, std::to_string(m_port));
    for(asio::ip::tcp::resolver::iterator iter = l_endpointResult.begin(); iter != l_endpointResult.end(); iter++)
    {
        asio::ip::tcp::endpoint endpoint = *iter;
        std::cout << endpoint << std::endl;
    }

    asio::error_code l_errorCode;
    asio::connect(m_tcpSocket, l_endpointResult, l_errorCode);
    if(l_errorCode.value() == 0)
    {
        return true;
    }
    else
    {
        qDebug() << "connect to ssdb server error, " << l_errorCode.value();
        return false;
    }
}

bool TCPClient::disConnect()
{
    asio::error_code l_errorCode;
    m_tcpSocket.shutdown(asio::socket_base::shutdown_both, l_errorCode);
    if(l_errorCode.value() == 0)
    {
        m_tcpSocket.close();
        return true;
    }
    else
    {
        qDebug() << "disconnect from ssdb server error, " << l_errorCode.value();
        return false;
    }
}

bool TCPClient::isConnected()
{
    return m_tcpSocket.is_open();
}

bool TCPClient::writeCount(const char* data, const int length)
{
    if(data == NULL || length <= 0)
    {
        qDebug() << "tcp client write count input argment is error";
        return false;
    }

    asio::error_code l_errorCode;
    int l_nWrite = m_tcpSocket.write_some(asio::buffer(data, length), l_errorCode);
    if(l_nWrite == length && l_errorCode.value() == 0)
    {
        return true;
    }
    else
    {
        qDebug() << "tcp client write error, errorCode = " << l_errorCode.value();
        return false;
    }
}

int  TCPClient::readCount(char* data, int maxLength)
{
    if(data == NULL || maxLength <= 0)
    {
        qDebug() << "tcp client read count input argment is error";
        return false;
    }

    std::time_t l_startTime = std::time(NULL);
    do
    {
        if(m_tcpSocket.available() > 0)
        {
            asio::error_code l_errorCode;
            int l_nRead = m_tcpSocket.read_some(asio::buffer(data, maxLength), l_errorCode);
            if(l_errorCode.value() == 0)
            {
                return l_nRead;
            }
            else
            {
                qDebug() << "tcp client read error, errorCode = " << l_errorCode.value();
                return -1;
            }
        }
    }
    while(std::time(NULL) <= (l_startTime + 2));
    return false;
}

int  TCPClient::readLine(char* data, int maxLength)
{
    if(data == NULL || maxLength <= 0)
    {
        qDebug() << "tcp client read line input argment is error";
        return false;
    }

    int l_readOffset = 0;
    std::time_t l_startTime = std::time(NULL);
    do
    {
        if(true == readCount(data + l_readOffset, 1))
        {
            l_readOffset++;
            if(data[l_readOffset - 1] == '\n')
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    while(std::time(NULL) <= (l_startTime + 2));
    return l_readOffset;
}

std::string TCPClient::host() const
{
    return m_host;
}

void TCPClient::setHost(const std::string &host)
{
    m_host = host;
}

int TCPClient::port() const
{
    return m_port;
}

void TCPClient::setPort(int port)
{
    m_port = port;
}
