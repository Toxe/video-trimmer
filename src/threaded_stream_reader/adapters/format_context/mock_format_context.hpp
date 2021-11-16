#pragma once

#include <array>

#include "format_context.hpp"

class MockFormatContext : public FormatContext {
    const int max_number_of_video_frames_to_return_ = 3 * 60;  // 3 seconds worth of frames at 60 FPS

    int read_stream_index_ = 0;
    std::array<int, 2> num_packets_ = {0};

public:
    [[nodiscard]] double stream_time_base(int stream_index) const override;

    [[nodiscard]] std::string format() const override;

    [[nodiscard]] std::unique_ptr<StreamInfo> find_best_stream(Factory* factory, StreamType type) override;
    [[nodiscard]] int read_frame(Packet* packet) override;
};
