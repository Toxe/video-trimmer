#pragma once

#include <optional>
#include <string>

int show_error(const std::string& error_message, std::optional<int> error_code = std::nullopt);
void die(const std::string& error_message);
