#pragma once

#include <memory>
#include <string_view>

#include "../adapters/format_context/format_context.hpp"
#include "../stream_info/stream_info.hpp"

struct AVStream;

class Factory {
public:
    [[nodiscard]] virtual std::unique_ptr<FormatContext> create_format_context(const std::string_view& filename) = 0;

    virtual ~Factory() = default;
};
