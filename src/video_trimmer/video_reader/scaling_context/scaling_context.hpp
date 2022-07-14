#pragma once

#include <memory>

#include "../codec_context/codec_context.hpp"
#include "../frame/frame.hpp"

namespace video_trimmer::video_reader::scaling_context {

class ScalingContext {
public:
    ScalingContext(codec_context::CodecContext* codec_context, int width, int height);
    ~ScalingContext();

    int scale(frame::Frame* frame);

private:
    class Impl;
    std::unique_ptr<Impl> impl_;
};

}  // namespace video_trimmer::video_reader::scaling_context
