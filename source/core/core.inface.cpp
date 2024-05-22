#include <cvAutoTrack.Core.h>

#include <map>
#include <string>

static std::map<std::string, callback_t> g_callbacks;

core_ptr CreateInstance()
{
    auto core = new cvAutoTrackCore();
    core->destroy = [](core_ptr core)
    { delete core; };
    core->init = []()
    { return 0; };
    core->release = []()
    { return 0; };
    core->start = []()
    { return 0; };
    core->stop = []()
    { return 0; };
    core->setConfig = [](in_string_ptr key, in_string_ptr value)
    { return 0; };
    core->getConfig = [](in_string_ptr key, out_string_ptr value, int size)
    { return 0; };
    core->setCallback = [](in_string_ptr callback_key, callback_t callback, void *user_data)
    { g_callbacks[callback_key] = callback; return 0; };
    core->removeCallback = [](in_string_ptr callback_key)
    { 
        auto it = g_callbacks.find(callback_key);
        if (it != g_callbacks.end())
            g_callbacks.erase(it);
        return 0;
    };
    core->call = [](in_string_ptr callback_key, in_string_ptr key, in_string_ptr value, void *user_data)
    { 
        auto it = g_callbacks.find(callback_key);
        if (it != g_callbacks.end())
            it->second(key, value, user_data);
        return 0;
    };
    return core;
}
