#pragma once

#include "fmt/chrono.h"
#include "fmt/format.h"

#include "frame_duration.hpp"
#include "playback_position.hpp"
#include "playback_time_point.hpp"
#include "video_duration.hpp"

template <>
struct fmt::formatter<video_trimmer::clock::FrameDuration> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end() + 1;
    }

    template <typename FormatContext>
    auto format(const video_trimmer::clock::FrameDuration& pos, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "(frame duration: {:.3f}s | {})", std::chrono::duration<double>(pos.microseconds()).count(), pos.microseconds());
    }
};

template <>
struct fmt::formatter<video_trimmer::clock::PlaybackPosition> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end() + 1;
    }

    template <typename FormatContext>
    auto format(const video_trimmer::clock::PlaybackPosition& pos, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "(@ {:.3f}s | {})", std::chrono::duration<double>(pos.microseconds()).count(), pos.microseconds());
    }
};

template <>
struct fmt::formatter<video_trimmer::clock::PlaybackTimePoint> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end() + 1;
    }

    template <typename FormatContext>
    auto format(const video_trimmer::clock::PlaybackTimePoint& pos, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "[= {:.3f}s | {}]", std::chrono::duration<double>(pos.microseconds()).count(), pos.microseconds());
    }
};

template <>
struct fmt::formatter<video_trimmer::clock::VideoDuration> {
    constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin())
    {
        return ctx.end() + 1;
    }

    template <typename FormatContext>
    auto format(const video_trimmer::clock::VideoDuration& pos, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return fmt::format_to(ctx.out(), "(video duration: {:.3f}s | {})", std::chrono::duration<double>(pos.microseconds()).count(), pos.microseconds());
    }
};
