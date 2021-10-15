#include "error.hpp"

#include <spdlog/spdlog.h>

extern "C" {
#include <libavutil/error.h>
}

int show_error(const std::string_view& error_message, std::optional<int> error_code)
{
    if (error_code.has_value()) {
        char buf[AV_ERROR_MAX_STRING_SIZE];
        av_strerror(error_code.value(), buf, AV_ERROR_MAX_STRING_SIZE);
        spdlog::error("error: {} ({})", error_message, buf);
        return error_code.value();
    } else {
        spdlog::error("error: {}", error_message);
        return -1;
    }
}
