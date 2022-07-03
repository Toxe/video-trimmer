#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "auto_delete_resource.hpp"
#include "format_context.hpp"
#include "threaded_stream_reader/stream_info/stream_info.hpp"

struct AVFormatContext;

class Packet;

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

    [[nodiscard]] std::unique_ptr<StreamInfo> find_best_stream(StreamType type);
    [[nodiscard]] int read_frame(Packet* packet);

private:
    AutoDeleteResource<AVFormatContext> format_context_;
};
