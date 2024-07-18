#pragma once
#include <vector>
#include <list>
#include <memory>
#include <string>
#include <utility>
#include <source_location>

namespace tianli::global
{
    template <size_t n> struct source_location
    {
        constexpr source_location(const char (&path)[n], size_t line, size_t col)
        {
            std::copy_n(path, n, this->path);
            this->line = line;
            this->col = col;
        }
        char path[n];
        size_t line;
        size_t col;
    };
    template <size_t n> struct error_message
    {
        constexpr error_message(const char (&msg)[n]) { std::copy_n(msg, n, this->msg); }
        char msg[n];
    };

    class error_invoker
    {
    public:
        struct location
        {
            std::string path;
            size_t line;
            size_t col;
            std::string error_msg;
        };
        inline static std::vector<location> locations = { { "default", 0, 0, "error" } };
        error_invoker(const std::string& path, size_t line, size_t col, const std::string& error_msg) { locations.emplace_back(location{ path, line, col, error_msg }); }
    };

    template <typename proxyer, source_location location, error_message message> class error_proxy
    {
    public:
        inline static proxyer proxy = { location.path, location.line, location.col, message.msg };
        template <typename err_fun, typename... Args> static auto callback(err_fun& f, Args&&... args) { return f(args...); }
    };

    inline int error_impl(const char* sz)
    {
        int index = 0;
        for (auto& [_, __, ___, message] : error_invoker::locations)
        {
            if (message == sz)
                break;
            index++;
        }
        return index;
    }

    #define deferr(msg)                                                                                                                                                                          \
        tianli::global::error_proxy<tianli::global::error_invoker, tianli::global::source_location(__FILE__, std::source_location::current().line(), std::source_location::current().column()), \
                                    tianli::global::error_message(msg)>::callback(tianli::global::error_impl, msg)

} // namespace tianli::global
