#pragma once

#include <string_view>

#include "auto_delete_ressource.hpp"
#include "format_context.hpp"

class FFmpegFormatContext : public FormatContext {
    auto_delete_ressource<AVFormatContext> format_context_ = {nullptr, nullptr};

public:
    FFmpegFormatContext(const std::string_view& filename);

    [[nodiscard]] double stream_time_base(int stream_index) const override;

    [[nodiscard]] const std::string format() const override;

    [[nodiscard]] std::unique_ptr<StreamInfo> find_best_stream(Factory* factory, StreamType type) override;
    [[nodiscard]] int read_frame(Packet* packet) override;
};
