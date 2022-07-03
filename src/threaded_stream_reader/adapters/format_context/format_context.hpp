#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "../../stream_info/stream_info.hpp"
#include "auto_delete_resource.hpp"
#include "format_context.hpp"

struct AVFormatContext;

class Factory;
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

    [[nodiscard]] std::unique_ptr<StreamInfo> find_best_stream(Factory* factory, StreamType type);
    [[nodiscard]] int read_frame(Packet* packet);

private:
    AutoDeleteResource<AVFormatContext> format_context_;
};
