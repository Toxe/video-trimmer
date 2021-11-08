#pragma once

#include <string_view>

#include "auto_delete_ressource.hpp"
#include "format_context.hpp"

class FFmpegFormatContext : public FormatContext {
    auto_delete_ressource<AVFormatContext> format_context_ = {nullptr, nullptr};

public:
    FFmpegFormatContext(const std::string_view& filename);

    [[nodiscard]] double stream_time_base(const int stream_index) const override;

    [[nodiscard]] virtual const std::string format() const override;

    [[nodiscard]] virtual std::unique_ptr<StreamInfo> find_best_stream(Factory* factory, const StreamType type) override;
    [[nodiscard]] virtual int read_frame(Packet* packet) override;
};
