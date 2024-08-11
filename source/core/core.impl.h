#pragma once
#include <global.error.h>
#include <global.convect.h>
#include <string>
#include <map>
#include <functional>

struct cvAutoTrackStringImpl
{
    std::string str;
    int set_string(std::string str){
        this->str = str;
        return 0;
    }
    int get_string(std::string &str){
        str = this->str;
        return 0;
    }
    int get_length(){
        return str.size();
    }
};

std::map<std::string, std::function<std::string(std::string)>> g_convectors = {
    {"utf-8", [](std::string str) { return str; }},
    {"gbk", [](std::string str) { return tianli::global::utf8_to_gbk(str); }},
};

struct cvAutoTrackErrorInfosImpl
{
    std::function<std::string(std::string)> current_convector{g_convectors["utf-8"]};
    std::string                             current_encoding{"utf-8"};
    int switch_encoding(std::string encoding){
        if (current_encoding == encoding)
            return 0;
        if (g_convectors.find(encoding) == g_convectors.end())
            return deferr("不存在指定的编码转换器");
        current_convector = g_convectors[encoding];
        current_encoding = encoding;
        return 0;
    }
};