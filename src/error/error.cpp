#include "error.hpp"

#include <array>
#include <stdexcept>

#include <fmt/core.h>

extern "C" {
#include <libavutil/error.h>
}

#include "logger/logger.hpp"

namespace video_trimmer::error {

int show_error(const std::string& error_message, std::optional<int> error_code)
{
    int ret_code = -1;

    if (error_code.has_value()) {
        std::array<char, AV_ERROR_MAX_STRING_SIZE> buf = {0};
        av_strerror(error_code.value(), buf.data(), AV_ERROR_MAX_STRING_SIZE);
        logger::log_error(fmt::format("{} ({})", error_message, buf.data()));
        ret_code = error_code.value();
    } else {
        logger::log_error(error_message);
    }

    return ret_code;
}

void die(const std::string& error_message)
{
    logger::log_critical(error_message);
    throw std::runtime_error(error_message);
}

}  // namespace video_trimmer::error
