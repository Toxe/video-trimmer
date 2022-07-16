#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "../packet/packet.hpp"

struct AVStream;

namespace video_trimmer::video_reader::format_context {

class FormatContext {
public:
    enum class StreamType {
        audio,
        video
    };

    explicit FormatContext(const std::string_view& filename);
    ~FormatContext();

    [[nodiscard]] double stream_time_base(int stream_index) const;

    [[nodiscard]] std::string format() const;

    [[nodiscard]] AVStream* find_best_stream(StreamType type);
    [[nodiscard]] int read_frame(packet::Packet* packet);

    [[nodiscard]] std::string_view filename() const;

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_reader::format_context
