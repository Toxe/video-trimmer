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

namespace video_trimmer::video_content_provider::format_context {

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
    [[nodiscard]] int read_frame(packet::Packet* packet);

private:
    AutoDeleteResource<AVFormatContext> format_context_;
};

}  // namespace video_trimmer::video_content_provider::format_context
