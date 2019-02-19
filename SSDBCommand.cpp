#include "SSDBCommand.h"

#define CRLF                    std::string("\n")
#define COMMAND_AUTH            std::string("4\nauth\n")
#define COMMAND_DEL             std::string("3\ndel\n")
#define COMMAND_KEYS            std::string("4\nkeys\n")
#define COMMAND_SCAN            std::string("4\nscan\n")
#define COMMAND_SET             std::string("3\nset\n")
#define COMMAND_SETX            std::string("4\nsetx\n")


static bool CommonResponseSpliteResultCheck(std::vector<std::string> splitResult)
{
    int splitSize = splitResult.size();
    if(splitSize % 2 == 0 && splitSize >= 4 && splitResult[splitSize-1] == "" && splitResult[splitSize-2] == ""
            && splitResult[1] == "ok" && std::atoi(splitResult[0].c_str()) == 2)
    {
        return true;
    }
    return false;
}

static std::string buildParam(std::string param)
{
    return std::to_string(param.size()) + CRLF + param + CRLF;
}

std::string SSDBCommand::Authbuild(std::string passwd)
{
    return COMMAND_AUTH + buildParam(passwd) + CRLF;
}

std::string SSDBCommand::SetBuild(std::string key, std::string value)
{
    return COMMAND_SET + buildParam(key) + buildParam(value) + CRLF;
}

bool SSDBCommand::SetResolve(const std::string str)
{
    std::vector<std::string> splitResult;
    if(true == Utility::string_split(str, "\n", splitResult))
    {
        if(true == CommonResponseSpliteResultCheck(splitResult))
        {
            return true;
        }
    }
    return false;
}

std::string SSDBCommand::SetxBuild(std::string key, std::string value, std::string expire)
{
    return COMMAND_SETX + buildParam(key) + buildParam(value) + buildParam(expire) + CRLF;
}

bool SSDBCommand::SetxResolve(const std::string str)
{
    std::vector<std::string> splitResult;
    if(true == Utility::string_split(str, "\n", splitResult))
    {
        if(true == CommonResponseSpliteResultCheck(splitResult))
        {
            return true;
        }
    }
    return false;
}

std::string SSDBCommand::DelBuild(std::string key)
{
    return COMMAND_DEL + buildParam(key) + CRLF;
}

bool SSDBCommand::DelResole(const std::string str)
{
    std::vector<std::string> splitResult;
    if(true == Utility::string_split(str, "\n", splitResult))
    {
        if(true == CommonResponseSpliteResultCheck(splitResult))
        {
            return true;
        }
    }
    return false;
}

std::string SSDBCommand::Keysbuild(std::string key_start, std::string key_end, std::string key_limit)
{
    return COMMAND_KEYS + buildParam(key_start) + buildParam(key_end) + buildParam(key_limit) + CRLF;
}

bool SSDBCommand::KeysResolve(const std::string str, std::vector<std::string>& keylist)
{
    std::vector<std::string> splitResult;
    if(true == Utility::string_split(str, "\n", splitResult))
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
            qDebug() << "CommonResponseSpliteResultCheck failed";
        }
    }
    else
    {
        qDebug() << "Utility::string_split failed";
    }
    return false;
}

std::string SSDBCommand::ScanBuild(std::string key_start, std::string key_end, std::string key_limit)
{
    return COMMAND_SCAN + buildParam(key_start) + buildParam(key_end) + buildParam(key_limit) + CRLF;
}

bool SSDBCommand::ScanResolve(const std::string str, std::vector<std::pair<std::string, std::string>>& keylist)
{
    std::vector<std::string> splitResult;
    if(true == Utility::string_split(str, "\n", splitResult))
    {
        int splitSize = splitResult.size();
        if(true == CommonResponseSpliteResultCheck(splitResult) && (splitSize - 4) %4 == 0)
        {
            keylist.clear();
            for(int index = 2; (index + 3) < (splitSize - 2);)
            {
                if(std::atoi(splitResult[index].c_str()) == (int)splitResult[index+1].size()
                        && std::atoi(splitResult[index+2].c_str()) == (int)splitResult[index+3].size())
                {
                    keylist.push_back(std::make_pair(splitResult[index+1], splitResult[index+3]));
                }
                index += 4;
            }
            return true;
        }
        else
        {
            qDebug() << "CommonResponseSpliteResultCheck failed";
        }
    }
    else
    {
        qDebug() << "Utility::string_split failed";
    }
    return false;
}



