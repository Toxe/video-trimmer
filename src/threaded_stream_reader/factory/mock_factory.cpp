#include "mock_factory.hpp"

#include "../adapters/codec_context/mock_codec_context.hpp"
#include "../adapters/format_context/mock_format_context.hpp"
#include "../adapters/frame/mock_frame.hpp"
#include "../adapters/packet/mock_packet.hpp"
#include "../adapters/scaling_context/mock_scaling_context.hpp"

std::unique_ptr<CodecContext> MockFactory::create_codec_context(AVStream* /*stream*/)
{
    return std::make_unique<MockCodecContext>();
}

std::unique_ptr<FormatContext> MockFactory::create_format_context(const std::string_view& /*filename*/)
{
    return std::make_unique<MockFormatContext>();
}

std::unique_ptr<ScalingContext> MockFactory::create_scaling_context(CodecContext* codec_context, const int width, const int height)
{
    return std::make_unique<MockScalingContext>(codec_context, width, height);
}

std::unique_ptr<Frame> MockFactory::create_frame(CodecContext* codec_context, const int scaled_width, const int scaled_height)
{
    return std::make_unique<MockFrame>(codec_context, scaled_width, scaled_height);
}

std::unique_ptr<Packet> MockFactory::create_packet()
{
    return std::make_unique<MockPacket>();
}
