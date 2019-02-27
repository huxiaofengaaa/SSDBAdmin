#ifndef SSDBCOMMAND_H
#define SSDBCOMMAND_H

#include <QDebug>
#include <string>
#include <vector>
#include "Utility.h"

class SSDBCommandKeyValue
{
public:
    virtual ~SSDBCommandKeyValue() = default;

    static std::string KeyValueBuildDel(std::string key);
    static std::string KeyValueBuildKeys(std::string key_start, std::string key_end, std::string key_limit);
    static std::string KeyValueBuildScan(std::string key_start, std::string key_end, std::string key_limit);
    static std::string KeyValueBuildSet(std::string key, std::string value);
    static std::string KeyValueBuildSetx(std::string key, std::string value, std::string expire);
    static std::string KeyValueBuildTTL(std::string key);

    static bool        KeyValueResolveDel(const std::vector<std::string> splitResult);
    static bool        KeyValueResolveKeys(const std::vector<std::string> splitResult, std::vector<std::string>& keylist);
    static bool        KeyValueResolveScan(const std::vector<std::string> splitResult, std::vector<std::pair<std::string, std::string>>& keylist);
    static bool        KeyValueResolveSet(const std::vector<std::string> splitResult);
    static bool        KeyValueResolveSetx(const std::vector<std::string> splitResult);
    static bool        KeyValueResolveTTL(const std::vector<std::string> splitResult, std::string& value);
};

class SSDBCommand: public SSDBCommandKeyValue
{
public:
    // server
    static std::string Authbuild(std::string passwd);
};

#endif // SSDBCOMMAND_H
