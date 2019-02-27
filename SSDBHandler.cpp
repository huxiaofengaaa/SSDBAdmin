#include "SSDBHandler.h"

SSDBHandler::SSDBHandler()
{
    m_tcpClient = new (std::nothrow) TCPClient();
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
        qDebug() << __FUNCTION__ << " - tcpclient is null";
        return false;
    }

    return m_tcpClient->isConnected();
}

bool SSDBHandler::connectToSSDB(const std::string p_host, const int p_port, const std::string)
{
    if(m_tcpClient == NULL)
    {
        qDebug() << __FUNCTION__ << " - tcpclient is null";
        return false;
    }

    bool l_ret = m_tcpClient->connect(p_host, p_port);
    if(l_ret == false)
    {
        qDebug() << __FUNCTION__ << " - connect error";
    }
    else
    {
        qDebug() << __FUNCTION__ << " - connect success";
    }
    return l_ret;
}

bool SSDBHandler::disConnectFromSSDB()
{
    if(m_tcpClient == NULL)
    {
        qDebug() << __FUNCTION__ << " - tcpclient is null";
        return false;
    }

    bool l_ret = m_tcpClient->disConnect();
    if(l_ret == false)
    {
        qDebug() << __FUNCTION__ << " - disConnect error";
    }
    else
    {
        qDebug() << __FUNCTION__ << " - disConnect success";
    }
    return l_ret;
}

bool SSDBHandler::getKeyValueLists(const std::string p_keyStart,
                                   const std::string p_keyEnd,
                                   const std::string p_keyLimit,
                                   KeyValueListType& p_keyValueItemLists)
{
    std::string l_command = SSDBCommand::KeyValueBuildScan(p_keyStart, p_keyEnd, p_keyLimit);

    if(sendSSDBCommandRequest(l_command) == false)
    {
        qDebug() << __FUNCTION__ << " - sendSSDBCommandRequest error";
        return false;
    }

    std::vector<std::string> l_response = readSSDBCommandResponse();
    if(l_response.size() <= 0)
    {
        qDebug() << __FUNCTION__ << " - readSSDBCommandResponse error";
        return false;
    }

    std::vector<std::pair<std::string, std::string>> l_keyValueLists;
    if(false == SSDBCommand::KeyValueResolveScan(l_response, l_keyValueLists))
    {
        qDebug() << __FUNCTION__ << " - ScanResolve error";
        return false;
    }

    for(auto iter = l_keyValueLists.begin(); iter != l_keyValueLists.end(); iter++)
    {
        std::string l_keyTTL;
        if(false == getKeyValueTTL((*iter).first, l_keyTTL))
        {
            qDebug() << __FUNCTION__ << " - getKeyValueTTL error, key = " << (*iter).first.c_str();
            continue;
        }
        p_keyValueItemLists.push_back(std::make_shared<KeyValueItem>((*iter).first,
                                                                     (*iter).second,
                                                                     l_keyTTL));
    }

    return true;
}

bool SSDBHandler::insertKeyValue(const std::string p_key,
                                 const std::string p_value,
                                 const std::string p_expire)
{
    std::string l_command;
    if(!p_expire.empty())
    {
        l_command = SSDBCommand::KeyValueBuildSetx(p_key, p_value, p_expire);
    }
    else
    {
        l_command = SSDBCommand::KeyValueBuildSet(p_key, p_value);
    }

    if(sendSSDBCommandRequest(l_command) == false)
    {
        qDebug() << __FUNCTION__ << " - sendSSDBCommandRequest error";
        return false;
    }

    std::vector<std::string> l_response = readSSDBCommandResponse();
    if(l_response.size() <= 0)
    {
        qDebug() << __FUNCTION__ << " - readSSDBCommandResponse error";
        return false;
    }

    if(!p_expire.empty())
    {
        if(false == SSDBCommand::KeyValueResolveSetx(l_response))
        {
            qDebug() << __FUNCTION__ << " - KeyValueResolveSetx error";
            return false;
        }
    }
    else
    {
        if(false == SSDBCommand::KeyValueResolveSet(l_response))
        {
            qDebug() << __FUNCTION__ << " - KeyValueResolveSet error";
            return false;
        }
    }

    return true;
}

bool SSDBHandler::deleteKeyValue(const std::string p_key)
{
    std::string l_command = SSDBCommand::KeyValueBuildDel(p_key);

    if(sendSSDBCommandRequest(l_command) == false)
    {
        qDebug() << __FUNCTION__ << " - sendSSDBCommandRequest error";
        return false;
    }

    std::vector<std::string> l_response = readSSDBCommandResponse();
    if(l_response.size() <= 0)
    {
        qDebug() << __FUNCTION__ << " - readSSDBCommandResponse error";
        return false;
    }

    if(false == SSDBCommand::KeyValueResolveDel(l_response))
    {
        qDebug() << __FUNCTION__ << " - KeyValueResolveDel error";
        return false;
    }

    return true;
}

bool SSDBHandler::getKeyValueTTL(const std::string p_key, std::string& p_ttl)
{
    std::string l_command = SSDBCommand::KeyValueBuildTTL(p_key);

    if(sendSSDBCommandRequest(l_command) == false)
    {
        qDebug() << __FUNCTION__ << " - sendSSDBCommandRequest error";
        return false;
    }

    std::vector<std::string> l_response = readSSDBCommandResponse();
    if(l_response.size() <= 0)
    {
        qDebug() << __FUNCTION__ << " - readSSDBCommandResponse error";
        return false;
    }

    if(false == SSDBCommand::KeyValueResolveTTL(l_response, p_ttl))
    {
        qDebug() << __FUNCTION__ << " - KeyValueResolveTTL error";
        return false;
    }

    return true;
}

bool SSDBHandler::sendSSDBCommandRequest(std::string p_command)
{
    if(m_tcpClient == NULL)
    {
        qDebug() << __FUNCTION__ << " - tcpclient is null";
        return false;
    }

    if(false == m_tcpClient->writeCount(p_command.c_str(), p_command.size()))
    {
        qDebug() << __FUNCTION__ << " - writeCount error";
        return false;
    }
    QString l_command = p_command.c_str();
    qDebug() << "send ssdb command success, " << l_command;
    return true;
}

std::vector<std::string> SSDBHandler::readSSDBCommandResponse()
{
    std::vector<std::string> l_ssdbResponse;
    if(m_tcpClient == NULL)
    {
        qDebug() << __FUNCTION__ << " - tcpclient is null";
        return l_ssdbResponse;
    }


    while(true)
    {
        char l_LineBuffer[1024 * 16] = { 0 };
        int l_readLineLength = m_tcpClient->readLine(l_LineBuffer, sizeof(l_LineBuffer));
        if(l_readLineLength <= 0)
        {
            qDebug() << __FUNCTION__ << " - readLine error";
            break;
        }
        else
        {
            std::string l_line = std::string(l_LineBuffer, l_readLineLength);
            l_ssdbResponse.push_back(l_line);
            if(l_line == "\n")
            {
                break;
            }
        }
    }

    std::string l_response;
    std::vector<std::string>::iterator iter;
    for(iter = l_ssdbResponse.begin(); iter != l_ssdbResponse.end(); iter++)
    {
        l_response += (*iter);
    }
    QString l_command = l_response.c_str();
    qDebug() << "read ssdb response success, " << l_command;

    return l_ssdbResponse;
}

