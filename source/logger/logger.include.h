#pragma once
#include <chrono>
#include <list>
#include <memory>
#include <string>
#include <utility>

namespace tianli::logger
{
    class error_type
    {
    public:
        int code;
        std::string msg;
        std::string location;
    };

    class error_info
    {
    public:
        std::chrono::system_clock::time_point time;
        std::chrono::system_clock::time_point begin_time;
        std::chrono::system_clock::time_point end_time;
        std::list<std::shared_ptr<error_type>> errors;

    public:
        error_info& operator=(const std::pair<int, std::string>& err_code_msg)
        {
            errors.emplace_back(std::make_shared<error_type>(error_type{ err_code_msg.first, err_code_msg.second }));
            return *this;
        }
        operator std::pair<int, std::string>() { return std::make_pair(errors.back()->code, errors.back()->msg); }
    };
    class error_resigter
    {
    public:
        std::list<std::shared_ptr<error_info>> error_list;
        std::shared_ptr<error_info> last_error;
    };

    // 日志基类
    class logger_interface
    {
    public:
        enum class level
        {
            debug,
            info,
            warn,
            error,
            fatal
        };
        enum class log_type
        {
            console,
            file,
            database
        };
    public:
        static std::shared_ptr<logger_interface> create(log_type type);
    public:
        logger_interface() = default;
        virtual void write(level lv, const char* msg) = 0;
        virtual void log(error_type error) = 0;
        // 用于启用一次计时测量
        virtual void perl(std::string perl_label) = 0;
        virtual void perl_end(std::string perl_label) = 0;
    };

} // namespace tianli::global
