#include "ffmpeg_factory.hpp"

#include "../adapters/codec_context/ffmpeg_codec_context.hpp"
#include "../adapters/format_context/ffmpeg_format_context.hpp"
#include "../adapters/frame/ffmpeg_frame.hpp"
#include "../adapters/packet/ffmpeg_packet.hpp"
#include "../adapters/scaling_context/ffmpeg_scaling_context.hpp"

std::unique_ptr<CodecContext> FFmpegFactory::create_codec_context(AVStream* stream)
{
    return std::make_unique<FFmpegCodecContext>(stream);
}

std::unique_ptr<FormatContext> FFmpegFactory::create_format_context(const std::string_view& filename)
{
    return std::make_unique<FFmpegFormatContext>(filename);
}

std::unique_ptr<ScalingContext> FFmpegFactory::create_scaling_context(CodecContext* codec_context, const int width, const int height)
{
    return std::make_unique<FFmpegScalingContext>(codec_context, width, height);
}

std::unique_ptr<Frame> FFmpegFactory::create_frame(CodecContext* codec_context, const int scaled_width, const int scaled_height)
{
    return std::make_unique<FFmpegFrame>(codec_context, scaled_width, scaled_height);
}

std::unique_ptr<Packet> FFmpegFactory::create_packet()
{
    return std::make_unique<FFmpegPacket>();
}
