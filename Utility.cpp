#include "Utility.h"

bool Utility::string_split(const std::string& p_src_string, const std::string& p_seperator, std::string& p_first_split, std::string& p_second_split)
{
    if(p_seperator.empty() == true || p_src_string.empty() == true)
    {
        p_first_split = "";
        p_second_split = "";
        return false;
    }
    std::string::size_type l_search_pos = p_src_string.find(p_seperator, 0);
    if(l_search_pos == std::string::npos)
    {
        p_first_split = p_src_string;
        p_second_split = "";
        return true;
    }
    else
    {
        p_first_split = p_src_string.substr(0, l_search_pos);
        p_second_split = p_src_string.substr(l_search_pos + p_seperator.size());
        return true;
    }
}

bool Utility::string_split(const std::string& p_src_string, const std::string& p_seperator, std::vector<std::string>& p_split_result)
{
    if(p_seperator.empty() == true || p_src_string.empty() == true)
    {
        p_split_result.clear();
        return false;
    }

    std::string first;
    std::string second;
    std::string src = p_src_string;
    do
    {
        if(true == string_split(src, p_seperator, first, second))
        {
            p_split_result.push_back(first);
            if(second.empty() == true)
            {
                if(first != src)
                {
                    p_split_result.push_back(second);
                }
                break;
            }
            else
            {
                src = second;
            }
        }
        else
        {
            return false;
        }
    }while(true);
    return true;
}

std::string Utility::string_trim(const std::string& p_src_string)
{
    int l_start_pos = 0;
    int l_end_pos = p_src_string.size() - 1;
    for(unsigned int i = 0 ; i < p_src_string.size() ; i++)
    {
        if(p_src_string[i] != ' ' && p_src_string[i] != '\t')
        {
            l_start_pos = i;
            break;
        }
    }
    for(int i = p_src_string.size() - 1 ; i >= 0 ; i--)
    {
        if(p_src_string[i] != ' ' && p_src_string[i] != '\t')
        {
            l_end_pos = i;
            break;
        }
    }
    return p_src_string.substr(l_start_pos, l_end_pos - l_start_pos + 1);
}

int Utility::string_count(const std::string& p_src_string, const std::string& p_sub_string)
{
    if(p_src_string.empty() == true || p_sub_string.empty() == true)
    {
        return -1;
    }

    int l_sub_count = 0;
    std::string::size_type l_offset = 0;
    while(l_offset < p_src_string.size())
    {
        std::string::size_type l_search_pos = p_src_string.find(p_sub_string, l_offset);
        if(l_search_pos == std::string::npos)
        {
            break;
        }
        else
        {
            l_sub_count++;
            l_offset += l_search_pos + p_sub_string.size();
        }
    }
    return l_sub_count;
}

bool Utility::string_startwith(const std::string& p_src_string, const std::string& p_sub_string)
{
    if(p_src_string.empty() == true || p_sub_string.empty() == true
            || p_sub_string.size() > p_src_string.size())
    {
        return false;
    }

    //const int l_src_size = p_src_string.size();
    const int l_sub_size = p_sub_string.size();
    for(auto i = 0 ; i < l_sub_size; i++)
    {
        if(p_sub_string[i] != p_src_string[i])
        {
            return false;
        }
    }
    return true;
}

bool Utility::string_endwith(const std::string& p_src_string, const std::string& p_sub_string)
{
    if(p_src_string.empty() == true || p_sub_string.empty() == true
            || p_sub_string.size() > p_src_string.size())
    {
        return false;
    }

    const int l_src_size = p_src_string.size();
    const int l_sub_size = p_sub_string.size();
    for(auto i = 0 ; i < l_sub_size; i++)
    {
        if(p_sub_string[i] != p_src_string[l_src_size - l_sub_size + i])
        {
            return false;
        }
    }
    return true;
}

bool Utility::string_replace(std::string& p_src_string, const std::string& p_sub_string, const std::string& p_new_string)
{
    if(p_src_string.empty() == true || p_sub_string.empty() == true || p_sub_string == p_new_string)
    {
        return false;
    }

    if(p_sub_string.size() > p_src_string.size())
    {
        return true;
    }

    while(true)
    {
        std::string::size_type l_search_pos = p_src_string.find(p_sub_string, 0);
        if(l_search_pos == std::string::npos)
        {
            break;
        }
        else
        {
            p_src_string = p_src_string.substr(0, l_search_pos) +
                    p_new_string + p_src_string.substr(l_search_pos + p_sub_string.size());
        }
    }
    return true;
}

bool Utility::string_toNumber(const std::string& p_src_string, int& p_number)
{
    p_number = 0;
    std::string l_src_string = string_trim(p_src_string);
    if(true == l_src_string.empty())
    {
        return false;
    }

    for(std::string::iterator iter = l_src_string.begin() ; iter != l_src_string.end(); iter++)
    {
        if(*iter >= '0' && *iter <= '9')
        {
            int l_number = *iter - '0';
            if(p_number == 0)
            {
                p_number = l_number;
            }
            else
            {
                p_number = 10 * p_number + l_number;
            }
        }
        else
        {
            if(iter == l_src_string.begin())
            {
                return false;
            }
            else
            {
                return true;
            }
        }
    }
    return true;
}

std::string Utility::string_Lower(const std::string& p_src_string)
{
    std::string l_result = p_src_string;
    for(unsigned int i = 0; i < l_result.size(); i++)
    {
        if(l_result[i] >= 'A' && l_result[i] <= 'Z')
        {
            l_result[i] += ('a' - 'A');
        }
    }
    return l_result;
}

std::string Utility::string_Upper(const std::string& p_src_string)
{
    std::string l_result = p_src_string;
    for(unsigned int i = 0; i < l_result.size(); i++)
    {
        if(l_result[i] >= 'a' && l_result[i] <= 'z')
        {
            l_result[i] += ('A' - 'a');
        }
    }
    return l_result;
}

std::string Utility::Hex(unsigned char ch, bool isUpper)
{
    std::string convert_result;
    const char *hex_book = (isUpper == true) ? "0123456789ABCDEF" : "0123456789abcdef";
    convert_result.push_back(hex_book[ch / 16]);
    convert_result.push_back(hex_book[ch % 16]);
    return convert_result;
}

std::string Utility::Hex(const unsigned char* str, int str_length, bool isUpper)
{
    std::string convert_result;
    try
    {
        if(str != NULL && str_length > 0)
        {
            for(int i = 0 ; i < str_length ; i++)
            {
                convert_result += Hex(str[i], isUpper);
            }
        }
    }
    catch(...)
    {

    }
    return convert_result;
}

std::string Utility::Hex(std::string& str, bool isUpper)
{
    return Hex((const unsigned char*)str.c_str(), str.size(), isUpper);
}

std::string toHexUTF8(char ch)
{
    std::string book = "0123456789ABCDEF";
    char buf[3] = { 0 };

    buf[0] = '%';
    buf[1] = book[(ch >> 4) & 0x0F];
    buf[0] = book[(ch >> 0) & 0x0F];
    return std::string(buf, 3);
}

std::string Utility::UriEncode(const std::string input, bool encodeSlash)
{
    std::string result;
    for(unsigned int i = 0 ; i < input.size() ; i++)
    {
        char ch = input[i];
        if ((ch >= 'A' && ch <= 'Z')
                || (ch >= 'a' && ch <= 'z')
                || (ch >= '0' && ch <= '9')
                || ch == '_'
                || ch == '-'
                || ch == '~'
                || ch == '.')
        {
            result += std::string(&ch, 1);
        }
        else if (ch == '/')
        {
            result += (encodeSlash ? std::string("%2F") : std::string(&ch, 1));
        }
        else
        {
            result += toHexUTF8(ch);
        }
    }
    return result;
}

std::string Utility::randomNumberString(int length)
{
    std::string result;
    int last_number = 0;
    while(length--)
    {
        std::srand(std::time(nullptr) + last_number);
        last_number = std::rand();
        result += std::to_string(last_number % 10);
    }
    return result;
}
