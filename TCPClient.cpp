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

//    asio::error_code l_errorCode;
//    asio::connect(m_tcpSocket, m_resolver.resolve(m_host.c_str(), std::to_string(m_port)), l_errorCode);
//    std::cout << "connect errorCode " << l_errorCode.value() << std::endl;

    return false;
}

bool TCPClient::disConnect()
{

    return false;
}

bool TCPClient::isConnected()
{

    return false;
}

bool TCPClient::writeCount(const char* data, const int length)
{

    return false;
}

int  TCPClient::readCount(char* data, int maxLength)
{

    return 0;
}

int  TCPClient::readLine(char* data, int maxLength)
{


    return 0;
}
