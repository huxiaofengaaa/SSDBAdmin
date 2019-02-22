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

    return false;
}

int  TCPClient::readCount(char* data, int maxLength)
{
    std::time_t l_readStartTimestamps = std::time(NULL);
    do
    {
        size_t l_countReadyRead = m_tcpSocket.available();
        if(l_countReadyRead < maxLength)
        {
            Utility::Msleep(50);
        }
        else
        {
            break;
        }
    }
    while(std::time(NULL) < (l_readStartTimestamps + 2));

    size_t l_countReadyRead = m_tcpSocket.available();
    if(l_countReadyRead > 0)
    {
        m_tcpSocket.read_some();
    }

    return 0;
}

int  TCPClient::readLine(char* data, int maxLength)
{
    std::time_t l_readStartTimestamps = std::time(NULL);
    do
    {
        size_t l_countReadyRead = m_tcpSocket.available();
        if(l_countReadyRead < maxLength)
        {
            Utility::Msleep(50);
        }
        else
        {
            break;
        }
    }
    while(std::time(NULL) < (l_readStartTimestamps + 2));

    size_t l_countReadyRead = m_tcpSocket.available();
    if(l_countReadyRead > 0)
    {
        asio::streambuf response;
        return asio::read_until(m_tcpSocket, response, "\n");
    }

    return 0;
}
