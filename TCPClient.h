#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QDebug>
#include <string>
#include <ctime>
#include <mutex>
#include <iostream>
#include "Utility.h"
#include "asio.hpp"

#define TCP_WAIT_FOR_CONNECT_TIMEOUT_MSEC        1000

class TCPClient
{
public:
    TCPClient(const std::string host = "", const int port = -1);
    ~TCPClient();
    bool                     connect(const std::string host, const int port);
    bool                     disConnect();
    bool                     isConnected();
    bool                     writeCount(const char* data, const int length);
    int                      readCount(char* data, int maxLength);
    int                      readLine(char* data, int maxLength);
    std::string              host() const;
    void                     setHost(const std::string &host);
    int                      port() const;
    void                     setPort(int port);

private:
    std::string              m_host;
    int                      m_port;
    asio::io_context         m_ioContext;
    asio::ip::tcp::resolver  m_resolver;
    asio::ip::tcp::socket    m_tcpSocket;
};

#endif // TCPCLIENT_H
