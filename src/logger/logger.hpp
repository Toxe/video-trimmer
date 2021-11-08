#pragma once

#include <string>

void log_init();

void log_trace(const std::string& s);
void log_debug(const std::string& s);
void log_info(const std::string& s);
void log_warn(const std::string& s);
void log_error(const std::string& s);
void log_critical(const std::string& s);
