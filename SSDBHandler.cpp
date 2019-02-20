#include "SSDBHandler.h"

SSDBHandler::SSDBHandler()
{
    m_tcpClient = new (std::nothrow) TCPClient(false);
}

SSDBHandler::~SSDBHandler()
{
    if(m_tcpClient)
    {
        delete m_tcpClient;
        m_tcpClient = NULL;
    }
}

bool SSDBHandler::isConnected()
{
    if(m_tcpClient == NULL)
    {
        qDebug() << "m_tcpClient == NULL";
        return false;
    }

    return m_tcpClient->isConnected();
}

bool SSDBHandler::connectToSSDB(const std::string p_host, const int p_port, const std::string)
{
    if(m_tcpClient == NULL)
    {
        qDebug() << "m_tcpClient == NULL";
        return false;
    }

    bool l_ret = m_tcpClient->connect(p_host, p_port);
    if(l_ret == false)
    {
        qDebug() << "connect to ssdb server error";
    }
    else
    {
        qDebug() << "connect to ssdb server success";
    }
    return l_ret;
}

bool SSDBHandler::disConnectFromSSDB()
{
    if(m_tcpClient == NULL)
    {
        qDebug() << "m_tcpClient == NULL";
        return false;
    }

    bool l_ret = m_tcpClient->disConnect();
    if(l_ret == false)
    {
        qDebug() << "disconnect from ssdb server error";
    }
    else
    {
        qDebug() << "disconnect from ssdb server success";
    }
    return l_ret;
}

bool SSDBHandler::getKeyValueLists(const std::string p_keyStart,
                                   const std::string p_keyEnd,
                                   const std::string p_keyLimit,
                                   std::vector<std::pair<std::string, std::string>>& p_keyValueLists)
{
    std::string l_command = SSDBCommand::ScanBuild(p_keyStart, p_keyEnd, p_keyLimit);

    if(sendSSDBCommandRequest(l_command) == false)
    {
        qDebug("send ssdb scan command error");
        return false;
    }

    std::string l_response = readSSDBCommandResponse();
    if(l_response.empty() == true)
    {
        qDebug("read ssdb scan command response error");
        return false;
    }

    return true;
}

bool SSDBHandler::insertKeyValue(const std::string p_key,
                                 const std::string p_value,
                                 const std::string p_expire)
{
    if(p_key.empty() == true || p_value.empty() == true)
    {
        qDebug() << "key & value can not be null";
        return false;
    }

    std::string l_command;
    if(!p_expire.empty())
    {
        l_command = SSDBCommand::SetxBuild(p_key, p_value, p_expire);
    }
    else
    {
        l_command = SSDBCommand::SetBuild(p_key, p_value);
    }

    if(sendSSDBCommandRequest(l_command) == false)
    {
        qDebug("send ssdb set/setx command error");
        return false;
    }
    return true;
}

bool SSDBHandler::deleteKeyValue(const std::string p_key)
{
    std::string l_command = SSDBCommand::DelBuild(p_key);

    if(sendSSDBCommandRequest(l_command) == false)
    {
        qDebug("send ssdb del command error");
        return false;
    }
    return true;
}

bool SSDBHandler::sendSSDBCommandRequest(std::string p_command)
{
    qDebug() << p_command.c_str();

    if(m_tcpClient == NULL)
    {
        qDebug() << "m_tcpClient == NULL";
        return false;
    }

    if(false == m_tcpClient->writeCount(p_command.c_str(), p_command.size()))
    {
        qDebug() << "send ssdb command error";
        return false;
    }
    return true;
}

std::string SSDBHandler::readSSDBCommandResponse()
{
    if(m_tcpClient == NULL)
    {
        qDebug() << "m_tcpClient == NULL";
        return false;
    }
    QByteArray response = m_tcpClient->readCount(2);
    qDebug() << response;
    return response.data();
}

