#include "logger.include.h"
#include "record/record.stdlog.h"

namespace tianli::global
{
    std::shared_ptr<logger_interface> create_logger(logger_interface::log_type type)
    {
        switch (type)
        {
        case logger_interface::log_type::console:
            return std::make_shared<record::std_logger>();
        //case logger_interface::log_type::console:
        //    return std::make_shared<console_logger>();
        //case logger_interface::log_type::file:
        //    return std::make_shared<file_logger>();
        //case logger_interface::log_type::database:
        //    return std::make_shared<database_logger>();
        default:
            return nullptr;
        }
    }
} // namespace tianli::logger
