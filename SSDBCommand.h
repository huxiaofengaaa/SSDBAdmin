#ifndef SSDBCOMMAND_H
#define SSDBCOMMAND_H

#include <QDebug>
#include <string>
#include <vector>
#include "Utility.h"

typedef enum
{
    SSDB_COMMAND_NONE,
    SSDB_COMMAND_CONNECT,
    SSDB_COMMAND_KV_SCAN,
    SSDB_COMMAND_KV_SET,
    SSDB_COMMAND_KV_DEL
}SSDBCommandType;

class SSDBCommand
{
public:
    // server
    static std::string Authbuild(std::string passwd);

    // key value
    static std::string DelBuild(std::string key);
    static bool        DelResole(const std::string str);
    static std::string Keysbuild(std::string key_start, std::string key_end, std::string key_limit);
    static bool        KeysResolve(const std::string str, std::vector<std::string>& keylist);
    static std::string ScanBuild(std::string key_start, std::string key_end, std::string key_limit);
    static bool        ScanResolve(const std::string str, std::vector<std::pair<std::string, std::string>>& keylist);
    static std::string SetBuild(std::string key, std::string value);
    static bool        SetResolve(const std::string str);
    static std::string SetxBuild(std::string key, std::string value, std::string expire);
    static bool        SetxResolve(const std::string str);


    // hashmap

    // sorted set

    // list
};

#endif // SSDBCOMMAND_H
