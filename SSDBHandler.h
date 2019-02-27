#ifndef SSDBHANDLER_H
#define SSDBHANDLER_H

#include "TCPClient.h"
#include "SSDBCommand.h"
#include <QThread>
#include <memory>


struct KeyValueItem
{
    std::string m_key;
    std::string m_value;
    std::string m_expire;
    KeyValueItem(std::string p_key, std::string p_value, std::string p_expire)
    {
        m_key = p_key;
        m_value = p_value;
        m_expire = p_expire;
    }
};

#define SSDB_READ_RESPONSE_TIMEOUT     2
#define KeyValueListType               std::vector<std::shared_ptr<KeyValueItem>>

class SSDBHandler
{
public:
    SSDBHandler();
    ~SSDBHandler();
    bool                     isConnected();

    bool                     connectToSSDB(const std::string p_host,
                                           const int p_port,
                                           const std::string passwd = "");

    bool                     disConnectFromSSDB();

    bool                     getKeyValueLists(const std::string p_keyStart,
                                              const std::string p_keyEnd,
                                              const std::string p_keyLimit,
                                              KeyValueListType& p_keyValueItemLists);

    bool                     insertKeyValue(const std::string p_key,
                                            const std::string p_value,
                                            const std::string p_expire);

    bool                     deleteKeyValue(const std::string p_key);

    bool                     getKeyValueTTL(const std::string p_key, std::string& p_ttl);
private:
    bool                     sendSSDBCommandRequest(std::string p_command);
    std::vector<std::string> readSSDBCommandResponse();
    TCPClient*               m_tcpClient;
};

#endif // SSDBHANDLER_H
