#pragma once

#include <memory>
#include <string>

struct AVFormatContext;

#include "../../stream_info/stream_info.hpp"

class Factory;
class Packet;

class FormatContext {
public:
    enum class StreamType {
        audio,
        video
    };

public:
    virtual ~FormatContext() = default;

    [[nodiscard]] virtual double stream_time_base(const int stream_index) const = 0;

    [[nodiscard]] virtual const std::string format() const = 0;

    [[nodiscard]] virtual std::unique_ptr<StreamInfo> find_best_stream(Factory* factory, const StreamType type) = 0;
    [[nodiscard]] virtual int read_frame(Packet* packet) = 0;
};
