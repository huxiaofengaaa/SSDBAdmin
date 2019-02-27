#ifndef SSDBHANDLER_H
#define SSDBHANDLER_H

#include "TCPClient.h"
#include "SSDBCommand.h"
#include <QThread>
#include <memory>

#define SSDB_READ_RESPONSE_TIMEOUT     2
#define KeyValueListType               std::vector<std::pair<std::string, std::string>>

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
                                              KeyValueListType& p_keyValueLists);

    bool                     insertKeyValue(const std::string p_key,
                                            const std::string p_value,
                                            const std::string p_expire);

    bool                     deleteKeyValue(const std::string p_key);
private:
    bool                     sendSSDBCommandRequest(std::string p_command);
    std::vector<std::string> readSSDBCommandResponse();
    TCPClient*               m_tcpClient;
};

#endif // SSDBHANDLER_H
