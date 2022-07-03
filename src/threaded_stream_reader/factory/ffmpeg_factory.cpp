#include "ffmpeg_factory.hpp"

#include "../adapters/format_context/ffmpeg_format_context.hpp"

std::unique_ptr<FormatContext> FFmpegFactory::create_format_context(const std::string_view& filename)
{
    return std::make_unique<FFmpegFormatContext>(filename);
}
