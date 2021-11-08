#include "logger.hpp"

#include <spdlog/spdlog.h>

void log_init()
{
    spdlog::set_level(spdlog::level::trace);
}

void log_trace(const std::string& s)
{
    spdlog::trace(s);
}

void log_debug(const std::string& s)
{
    spdlog::debug(s);
}

void log_info(const std::string& s)
{
    spdlog::info(s);
}

void log_warn(const std::string& s)
{
    spdlog::warn(s);
}

void log_error(const std::string& s)
{
    spdlog::error(s);
}

void log_critical(const std::string& s)
{
    spdlog::critical(s);
}
