#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <ctime>
#include <vector>
#include <thread>

class Utility
{
public:
    static bool        string_split(const std::string& p_src_string, const std::string& p_seperator, std::string& p_first_split, std::string& p_second_split);
    static bool        string_split(const std::string& p_src_string, const std::string& p_seperator, std::vector<std::string>& p_split_result);
    static std::string string_trim(const std::string& p_src_string);
    static int         string_count(const std::string& p_src_string, const std::string& p_sub_string);
    static bool        string_startwith(const std::string& p_src_string, const std::string& p_sub_string);
    static bool        string_endwith(const std::string& p_src_string, const std::string& p_sub_string);
    static bool        string_replace(std::string& p_src_string, const std::string& p_sub_string, const std::string& p_new_string);
    static bool        string_toNumber(const std::string& p_src_string, int& number);
    static std::string string_Lower(const std::string& p_src_string);
    static std::string string_Upper(const std::string& p_src_string);
    static std::string Hex(std::string& str, bool isUpper = false);
    static std::string Hex(const unsigned char* str, int str_length, bool isUpper = false);
    static std::string Hex(unsigned char ch, bool isUpper = false);
    static std::string UriEncode(const std::string input, bool encodeSlash);
    static std::string randomNumberString(int length);
    static void        Sleep(int p_sec);
    static void        Msleep(int p_msec);
};

#endif // UTILITY_H
