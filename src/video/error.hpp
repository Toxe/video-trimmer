#pragma once

#include <optional>
#include <string_view>

int show_error(const std::string_view& error_message, std::optional<int> error_code = std::nullopt);
