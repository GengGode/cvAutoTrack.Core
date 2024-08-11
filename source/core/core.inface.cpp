#include <cvAutoTrack.Core.h>
#include <global.error.h>

#include "core.impl.h"

#include <map>
#include <string>

string_ptr create_string()
{
    auto string = new cvAutoTrackString();
    string->impl = new cvAutoTrackStringImpl();
    string->destroy = [](string_ptr string)
    {
        delete string->impl;
        delete string;
    };

    string->set_string = [](string_ptr string, in_string_ptr str)
    {
        return string->impl->set_string(str);
    };
    string->get_string = [](string_ptr string, out_string_ptr str, int size)
    {
        if (string->impl->str.size() > size)
            return deferr("缓冲区大小不足");
        std::copy_n(string->impl->str.c_str(), string->impl->str.size(), str);
        return 0;
    };
    string->get_length = [](string_ptr string)
    {
        return string->impl->get_length();
    };
    return string;
}

// static std::map<std::string, callback_t> g_callbacks;
error_infos_ptr create_error_infos()
{
    auto error_infos = new cvAutoTrackErrorInfos();
    error_infos->impl = new cvAutoTrackErrorInfosImpl();
    error_infos->destroy = [](error_infos_ptr error_infos)
    {
        delete error_infos->impl;
        delete error_infos;
    };

    error_infos->set_infos_encoding = [](error_infos_ptr error_infos, in_string_ptr encoding)
    {
        return error_infos->impl->switch_encoding(encoding);
    };
    error_infos->get_infos_encoding = [](error_infos_ptr error_infos, string_ptr* encoding)
    { 
        auto str = create_string();
        str->impl->set_string(error_infos->impl->current_encoding);
        *encoding = str;
        return 0;
    };
    error_infos->get_info = [](error_infos_ptr error_infos, int index, string_ptr* info)
    { 
        if (index >= tianli::global::error_invoker::locations.size())
            return deferr("不存在的错误码编号");
        auto &location = tianli::global::error_invoker::locations[index];
        std::string error_msg = error_infos->impl->current_convector(location.error_msg);
        auto str = create_string();
        str->impl->set_string(error_msg);
        *info = str;
        return 0;
    };
    error_infos->get_info_raw = [](error_infos_ptr error_infos, int index, string_ptr* info)
    {
        if (index >= tianli::global::error_invoker::locations.size())
            return deferr("不存在的错误码编号");
        auto &location = tianli::global::error_invoker::locations[index];
        auto str = create_string();
        str->impl->set_string(location.error_msg);
        *info = str;
        return 0;
    };  
    error_infos->get_info_count = [](int *count)
    {
        *count = tianli::global::error_invoker::locations.size();
        return 0;
    };
    return error_infos;
}

core_ptr CreateInstance()
{
    auto core = new cvAutoTrackCore();
    core->destroy = [](core_ptr core)
    { delete core; };

    core->create_error_infos = []()
    { return create_error_infos(); };
    core->create_context = []()
    {
        auto context = new cvAutoTrackContext();
        //context->impl = new cvAutoTrackContextImpl();
        context->destroy = [](context_ptr context)
        {
            delete context->impl;
            delete context;
        };
        return context;
    };

    return core;
}
