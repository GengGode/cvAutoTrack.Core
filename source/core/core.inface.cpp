#include <cvAutoTrack.Core.h>
#include <global.error.h>

#include "core.impl.h"

#include <map>
#include <string>

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
    error_infos->get_infos_encoding = [](error_infos_ptr error_infos, out_string_ptr encoding, int size)
    { 
        if (error_infos->impl->current_encoding.size() > size)
            return deferr("缓冲区大小不足");
        std::copy_n(error_infos->impl->current_encoding.c_str(), error_infos->impl->current_encoding.size(), encoding);
        return 0;
        
    };
    error_infos->get_info = [](error_infos_ptr error_infos, int index, out_string_ptr info, int size)
    { 
        if (index >= tianli::global::error_invoker::locations.size())
            return deferr("不存在的错误码编号");
        auto &location = tianli::global::error_invoker::locations[index];
        std::string error_msg = error_infos->impl->current_convector(location.error_msg);
        if (error_msg.size() > size)
            return deferr("缓冲区大小不足");
        std::copy_n(error_msg.c_str(), error_msg.size(), info);
        return 0;
    };
    error_infos->get_info_raw = [](error_infos_ptr error_infos, int index, out_string_ptr info, int size)
    {
        if (index >= tianli::global::error_invoker::locations.size())
            return deferr("不存在的错误码编号");
        auto &location = tianli::global::error_invoker::locations[index];
        if (location.error_msg.size() > size)
            return deferr("缓冲区大小不足");
        std::copy_n(location.error_msg.c_str(), location.error_msg.size(), info);
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
