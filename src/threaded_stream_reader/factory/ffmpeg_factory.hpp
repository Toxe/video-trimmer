#pragma once

#include "factory.hpp"

class FFmpegFactory : public Factory {
public:
    [[nodiscard]] std::unique_ptr<FormatContext> create_format_context(const std::string_view& filename) override;
};
