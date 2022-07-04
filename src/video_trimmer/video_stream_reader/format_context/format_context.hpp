#pragma once

#include <memory>
#include <string>
#include <string_view>

extern "C" {
#include "libavformat/avformat.h"
}

#include "../packet/packet.hpp"
#include "auto_delete_resource.hpp"

struct AVFormatContext;

class FormatContext {
public:
    enum class StreamType {
        audio,
        video
    };

public:
    FormatContext(const std::string_view& filename);

    [[nodiscard]] double stream_time_base(int stream_index) const;

    [[nodiscard]] std::string format() const;

    [[nodiscard]] AVStream* find_best_stream(StreamType type);
    [[nodiscard]] int read_frame(Packet* packet);

private:
    AutoDeleteResource<AVFormatContext> format_context_;
};
