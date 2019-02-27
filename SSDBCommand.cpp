#include "SSDBCommand.h"

#define CRLF                    std::string("\n")
#define COMMAND_DEL             std::string("3\ndel\n")
#define COMMAND_KEYS            std::string("4\nkeys\n")
#define COMMAND_SCAN            std::string("4\nscan\n")
#define COMMAND_SET             std::string("3\nset\n")
#define COMMAND_SETX            std::string("4\nsetx\n")
#define COMMAND_TTL             std::string("3\nttl\n")


static bool CommonResponseSpliteResultCheck(std::vector<std::string> splitResult)
{
    int splitSize = splitResult.size();
    if(splitSize % 2 != 1 || splitSize < 3)
    {
        qDebug() << __FUNCTION__ << " - response line size error, " << splitSize;
        return false;
    }

    if(splitResult[1] != "ok\n" || std::atoi(splitResult[0].c_str()) != 2)
    {
        qDebug() << __FUNCTION__ << " - response staus error, "
                 << splitResult[0].c_str() << " "
                 << splitResult[1].c_str();
        return false;
    }

    if(splitResult[splitSize-1] != "\n" && splitResult[splitSize-2] != "\n")
    {
        qDebug() << __FUNCTION__ << " - response end line error";
        return false;
    }

    return true;
}

static std::string buildParam(std::string param)
{
    return std::to_string(param.size()) + CRLF + param + CRLF;
}

std::string SSDBCommandKeyValue::KeyValueBuildDel(std::string key)
{
    return COMMAND_DEL + buildParam(key) + CRLF;
}

std::string SSDBCommandKeyValue::KeyValueBuildKeys(std::string key_start, std::string key_end, std::string key_limit)
{
    return COMMAND_KEYS + buildParam(key_start) + buildParam(key_end) + buildParam(key_limit) + CRLF;
}

std::string SSDBCommandKeyValue::KeyValueBuildScan(std::string key_start, std::string key_end, std::string key_limit)
{
    return COMMAND_SCAN + buildParam(key_start) + buildParam(key_end) + buildParam(key_limit) + CRLF;
}

std::string SSDBCommandKeyValue::KeyValueBuildSet(std::string key, std::string value)
{
    return COMMAND_SET + buildParam(key) + buildParam(value) + CRLF;
}

std::string SSDBCommandKeyValue::KeyValueBuildSetx(std::string key, std::string value, std::string expire)
{
    return COMMAND_SETX + buildParam(key) + buildParam(value) + buildParam(expire) + CRLF;
}

std::string SSDBCommandKeyValue::KeyValueBuildTTL(std::string key)
{
    return COMMAND_TTL + buildParam(key) + CRLF;
}

bool SSDBCommandKeyValue::KeyValueResolveDel(const std::vector<std::string> splitResult)
{
    return CommonResponseSpliteResultCheck(splitResult);
}

bool SSDBCommandKeyValue::KeyValueResolveKeys(const std::vector<std::string> splitResult, std::vector<std::string>& keylist)
{
    int splitSize = splitResult.size();
    if(true == CommonResponseSpliteResultCheck(splitResult))
    {
        keylist.clear();
        for(int index = 2; (index + 1) < (splitSize - 2);)
        {
            if(std::atoi(splitResult[index].c_str()) == (int)splitResult[index+1].size())
            {
                keylist.push_back(splitResult[index+1]);
            }
            index += 2;
        }
        return true;
    }
    else
    {
        qDebug() << __FUNCTION__ << " - CommonResponseSpliteResultCheck error";
        return false;
    }
}

bool SSDBCommandKeyValue::KeyValueResolveScan(const std::vector<std::string> splitResult,
                                              std::vector<std::pair<std::string, std::string>>& keylist)
{
    int splitSize = splitResult.size();
    if(true == CommonResponseSpliteResultCheck(splitResult))
    {
        if((splitSize - 3) % 4 == 0)
        {
            keylist.clear();
            for(int index = 2; (index + 3) < (splitSize - 1); index += 4)
            {
                int l_keyLength     = std::atoi(splitResult[index].c_str());
                std::string l_key   = splitResult[index+1].substr(0, l_keyLength);
                int l_valueLength   = std::atoi(splitResult[index+2].c_str());
                std::string l_value = splitResult[index+3].substr(0, l_valueLength);
                keylist.push_back(std::make_pair(l_key, l_value));
            }
            return true;
        }
        else
        {
            qDebug() << __FUNCTION__ << " - splitResult error, " << splitSize;
            return false;
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << " - CommonResponseSpliteResultCheck error";
        return false;
    }
}

bool SSDBCommandKeyValue::KeyValueResolveSet(const std::vector<std::string> splitResult)
{
    return CommonResponseSpliteResultCheck(splitResult);
}

bool SSDBCommandKeyValue::KeyValueResolveSetx(const std::vector<std::string> splitResult)
{
    return CommonResponseSpliteResultCheck(splitResult);
}

bool SSDBCommandKeyValue::KeyValueResolveTTL(const std::vector<std::string> splitResult, std::string& value)
{
    int splitSize = splitResult.size();
    if(true == CommonResponseSpliteResultCheck(splitResult))
    {
        if(splitSize == 5 && std::atoi(splitResult[2].c_str()) == (splitResult[3].size() - 1))
        {
            value = splitResult[3].substr(0, std::atoi(splitResult[2].c_str()));
            return true;
        }
        else
        {
            qDebug() << __FUNCTION__ << " - splitSize error";
            return false;
        }
    }
    else
    {
        qDebug() << __FUNCTION__ << " - CommonResponseSpliteResultCheck error";
        return false;
    }
}


