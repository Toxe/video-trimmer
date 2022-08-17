#include "catch/fakeit.hpp"
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"

#include "../logger/logger.hpp"
#include "video_player.hpp"

using namespace std::literals;

namespace video_trimmer::video_player {

TEST_CASE("video_player::VideoPlayer")
{
    GIVEN("a new VideoPlayer")
    {
        WHEN("no file has been opened")
        {
            VideoPlayer video_player(false);

            THEN("it does not have an open file")
            {
                CHECK(!video_player.has_open_file());
            }

            THEN("it is not playing")
            {
                CHECK(!video_player.has_started_playing());
                CHECK(!video_player.has_received_first_real_frame());
                CHECK(!video_player.is_playing());
            }

            THEN("the current and previous playback positions are 0.0")
            {
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));
            }
        }

        WHEN("opening a file")
        {
            fakeit::Mock<video_file::VideoFile> mock_video_file;
            fakeit::Fake(Dtor(mock_video_file));
            fakeit::Fake(Method(mock_video_file, set_dump_first_frame));
            fakeit::When(Method(mock_video_file, filename)).AlwaysReturn("test.mov");
            fakeit::When(Method(mock_video_file, is_open)).AlwaysReturn(true);

            VideoPlayer video_player(false);
            video_player.play_file(std::unique_ptr<video_file::VideoFile>(&mock_video_file.get()), std::chrono::steady_clock::now());

            THEN("it has an open file")
            {
                CHECK(video_player.has_open_file());
            }

            THEN("it is not playing")
            {
                CHECK(!video_player.has_started_playing());
                CHECK(!video_player.has_received_first_real_frame());
                CHECK(!video_player.is_playing());
            }
        }
    }

    GIVEN("a VideoPlayer with an open file")
    {
        fakeit::Mock<video_file::VideoFile> mock_video_file;
        fakeit::Fake(Dtor(mock_video_file));
        fakeit::Fake(Method(mock_video_file, set_dump_first_frame));
        fakeit::When(Method(mock_video_file, filename)).AlwaysReturn("test.mov");
        fakeit::When(Method(mock_video_file, is_open)).AlwaysReturn(true);

        VideoPlayer video_player(false);
        video_player.play_file(std::unique_ptr<video_file::VideoFile>(&mock_video_file.get()), std::chrono::steady_clock::now());

        WHEN("calling start() and stop()")
        {
            THEN("it has started and stopped playing")
            {
                video_player.start(std::chrono::steady_clock::now());

                CHECK(video_player.has_started_playing());
                CHECK(video_player.is_playing());

                video_player.stop();

                CHECK(video_player.has_started_playing());
                CHECK(!video_player.is_playing());
            }
        }
    }

    GIVEN("playback position of 0.0, without an open file")
    {
        VideoPlayer video_player(false);

        const auto now = std::chrono::steady_clock::now();

        WHEN("increasing time")
        {
            THEN("playback position stays at 0.0")
            {
                video_player.update_time(now);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 5ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 10ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));
            }
        }
    }

    GIVEN("playback position of 0.0, with an open file")
    {
        const auto now = std::chrono::steady_clock::now();

        fakeit::Mock<video_file::VideoFile> mock_video_file;
        fakeit::Fake(Dtor(mock_video_file));
        fakeit::Fake(Method(mock_video_file, set_dump_first_frame));
        fakeit::When(Method(mock_video_file, filename)).AlwaysReturn("test.mov");
        fakeit::When(Method(mock_video_file, is_open)).AlwaysReturn(true);

        VideoPlayer video_player(false);
        video_player.play_file(std::unique_ptr<video_file::VideoFile>(&mock_video_file.get()), now);

        WHEN("increasing time, has not started playing")
        {
            THEN("playback position stays at 0.0")
            {
                video_player.update_time(now);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 5ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 10ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));
            }
        }

        WHEN("increasing time and is playing, has not received first real frame")
        {
            THEN("playback position stays at 0.0")
            {
                video_player.start(now);
                video_player.set_has_received_first_real_frame(false);

                video_player.update_time(now);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 5ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 10ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));
            }
        }

        WHEN("increasing time and is playing, has received first real frame")
        {
            THEN("playback position increases")
            {
                video_player.start(now);
                video_player.set_has_received_first_real_frame(true);

                video_player.update_time(now);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 5ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.005));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 10ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.010));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.005));

                video_player.update_time(now + 15ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.015));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.010));
            }
        }

        WHEN("increasing time and is paused, has not received first real frame")
        {
            THEN("playback position stays at 0.0")
            {
                video_player.start(now);
                video_player.stop();
                video_player.set_has_received_first_real_frame(false);

                video_player.update_time(now);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 5ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 10ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));
            }
        }

        WHEN("increasing time and is paused, has received first real frame")
        {
            THEN("playback position stays at 0.0")
            {
                video_player.start(now);
                video_player.stop();
                video_player.set_has_received_first_real_frame(true);

                video_player.update_time(now);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 5ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 10ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));
            }
        }

        WHEN("pausing after playing for a while, has not received first real frame")
        {
            THEN("playback position stays at 0.0")
            {
                video_player.start(now);
                video_player.set_has_received_first_real_frame(false);

                video_player.update_time(now);
                video_player.update_time(now + 5ms);
                video_player.update_time(now + 10ms);

                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.stop();

                video_player.update_time(now + 15ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));

                video_player.update_time(now + 20ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.0));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.0));
            }
        }

        WHEN("pausing after playing for a while, has received first real frame")
        {
            THEN("playback position stays at the moment of pause")
            {
                video_player.start(now);
                video_player.set_has_received_first_real_frame(true);

                // start --> playback position increases
                video_player.update_time(now);
                video_player.update_time(now + 5ms);
                video_player.update_time(now + 10ms);

                CHECK(video_player.playback_position() == Catch::Approx(0.010));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.005));

                // pause --> playback position freezes
                video_player.toggle_pause();

                video_player.update_time(now + 15ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.010));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.005));

                video_player.update_time(now + 20ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.010));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.005));

                video_player.update_time(now + 25ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.010));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.005));

                // unpause --> playback position continues
                video_player.toggle_pause();

                video_player.update_time(now + 30ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.015));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.010));

                video_player.update_time(now + 35ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.020));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.015));

                // pause --> playback position freezes
                video_player.toggle_pause();

                video_player.update_time(now + 40ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.020));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.015));

                video_player.update_time(now + 45ms);
                CHECK(video_player.playback_position() == Catch::Approx(0.020));
                CHECK(video_player.prev_playback_position() == Catch::Approx(0.015));
            }
        }
    }

    GIVEN("a VideoPlayer that is playing a file from the beginning")
    {
        const Size size{640, 480};
        const video_file::PixelFormat pixel_format{1};
        const auto now = std::chrono::steady_clock::now();

        fakeit::Mock<video_file::VideoFile> mock_video_file;
        fakeit::Fake(Dtor(mock_video_file));
        fakeit::Fake(Method(mock_video_file, set_dump_first_frame));
        fakeit::When(Method(mock_video_file, filename)).AlwaysReturn("test.mov");
        fakeit::When(Method(mock_video_file, is_open)).AlwaysReturn(true);
        fakeit::When(Method(mock_video_file, seek_position)).AlwaysReturn(true);

        VideoPlayer video_player(false);
        video_player.play_file(std::unique_ptr<video_file::VideoFile>(&mock_video_file.get()), now);
        video_player.start(now);

        logger::WithLogLevel with_log_level(logger::LogLevel::trace);

        WHEN("reading frames while not increasing time")
        {
            THEN("return the first available frame and wait for time to progress")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); });

                // return the first available frame at 0.0
                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));

                // wait, because time has not progressed enough to return frames 0.017 or newer
                CHECK(video_player.next_frame(now) == nullptr);
                CHECK(video_player.next_frame(now) == nullptr);
            }
        }

        WHEN("reading frames while increasing time enough for the next frame")
        {
            THEN("return all available frames")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); });

                // progress time enough to return each frame
                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 20ms)->timestamp() == Catch::Approx(0.017));
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(0.033));
                CHECK(video_player.next_frame(now + 60ms)->timestamp() == Catch::Approx(0.050));
            }
        }

        WHEN("reading frames while time increases only in small increments")
        {
            THEN("wait before able to return all frames")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); });

                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 10ms) == nullptr);
                CHECK(video_player.next_frame(now + 15ms) == nullptr);
                CHECK(video_player.next_frame(now + 20ms)->timestamp() == Catch::Approx(0.017));
                CHECK(video_player.next_frame(now + 25ms) == nullptr);
                CHECK(video_player.next_frame(now + 30ms) == nullptr);
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(0.033));
                CHECK(video_player.next_frame(now + 45ms) == nullptr);
                CHECK(video_player.next_frame(now + 55ms)->timestamp() == Catch::Approx(0.050));
            }
        }

        WHEN("reading frames while time progresses too slow")
        {
            THEN("drop frames that are too old")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.067, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.083, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.100, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.117, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.133, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.150, 1.0 / 60.0, 'P'); });

                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(0.033));  // skip 0.017
                CHECK(video_player.next_frame(now + 80ms)->timestamp() == Catch::Approx(0.067));  // skip 0.050
                CHECK(video_player.next_frame(now + 120ms)->timestamp() == Catch::Approx(0.117));  // skip 0.083 and 0.100
                CHECK(video_player.next_frame(now + 160ms)->timestamp() == Catch::Approx(0.150));  // skip 0.133
            }
        }

        WHEN("skipping backward to the beginning before reading any frames")
        {
            THEN("return all available frames")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); });

                fakeit::When(Method(mock_video_file, duration)).AlwaysReturn(20.0f);

                video_player.jump_backward(now);

                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 20ms)->timestamp() == Catch::Approx(0.017));
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(0.033));
                CHECK(video_player.next_frame(now + 60ms)->timestamp() == Catch::Approx(0.050));
            }
        }

        WHEN("skipping backward to the beginning after reading the first frames")
        {
            THEN("return all available frames")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); });

                fakeit::When(Method(mock_video_file, duration)).AlwaysReturn(20.0f);

                // read the first 4 frames
                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 20ms)->timestamp() == Catch::Approx(0.017));
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(0.033));
                CHECK(video_player.next_frame(now + 60ms)->timestamp() == Catch::Approx(0.050));

                // skip back to the beginning and read the first 4 frames again
                video_player.jump_backward(now + 70ms);

                CHECK(video_player.next_frame(now + 80ms)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 100ms)->timestamp() == Catch::Approx(0.017));
                CHECK(video_player.next_frame(now + 120ms)->timestamp() == Catch::Approx(0.033));
                CHECK(video_player.next_frame(now + 140ms)->timestamp() == Catch::Approx(0.050));
            }
        }

        WHEN("skipping forward to the middle of the video before reading any frames")
        {
            THEN("return the frames at the middle of the video")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.050, 1.0 / 60.0, 'P'); });

                fakeit::When(Method(mock_video_file, duration)).AlwaysReturn(20.0f);

                video_player.jump_forward(now);

                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(10.000));
                CHECK(video_player.next_frame(now + 20ms)->timestamp() == Catch::Approx(10.017));
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(10.033));
                CHECK(video_player.next_frame(now + 60ms)->timestamp() == Catch::Approx(10.050));
            }
        }

        WHEN("skipping forward to the middle of the video after reading the first frames")
        {
            THEN("return the frames at the middle of the video")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.067, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.083, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.100, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 10.117, 1.0 / 60.0, 'P'); });

                fakeit::When(Method(mock_video_file, duration)).AlwaysReturn(20.0f);

                // read the first 4 frames
                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 20ms)->timestamp() == Catch::Approx(0.017));
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(0.033));
                CHECK(video_player.next_frame(now + 60ms)->timestamp() == Catch::Approx(0.050));

                // skip forward to the middle of the video and read the next 4 frames
                video_player.jump_forward(now + 70ms);  // jump --> 10.60

                CHECK(video_player.next_frame(now + 80ms)->timestamp() == Catch::Approx(10.067));
                CHECK(video_player.next_frame(now + 100ms)->timestamp() == Catch::Approx(10.083));
                CHECK(video_player.next_frame(now + 120ms)->timestamp() == Catch::Approx(10.100));
                CHECK(video_player.next_frame(now + 140ms)->timestamp() == Catch::Approx(10.117));
            }
        }

        WHEN("skipping forward to the end of the video before reading any frames")
        {
            THEN("return the last frame")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.850, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.867, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.883, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.900, 1.0 / 60.0, 'P'); })
                    .Do([] { return nullptr; });

                fakeit::When(Method(mock_video_file, duration)).AlwaysReturn(5.0f);

                // skip to the end
                video_player.jump_forward(now);  // --> jump 5.0

                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(4.900));
            }
        }

        WHEN("skipping forward to the end of the video after reading the first frames")
        {
            THEN("return the last frame")
            {
                fakeit::When(Method(mock_video_file, read_next_frame))
                    .Do([] { return nullptr; })  // decoder needs a moment before returning the first real frame
                    .Do([] { return nullptr; })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.000, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.017, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.033, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 0.050, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.850, 1.0 / 60.0, 'I'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.867, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.883, 1.0 / 60.0, 'P'); })
                    .Do([=] { return video_file::Frame::create_video_frame(size, pixel_format, 1837735, 4.900, 1.0 / 60.0, 'P'); })
                    .Do([] { return nullptr; });

                fakeit::When(Method(mock_video_file, duration)).AlwaysReturn(5.0f);

                // read the first 4 frames
                CHECK(video_player.next_frame(now)->timestamp() == Catch::Approx(0.0));
                CHECK(video_player.next_frame(now + 20ms)->timestamp() == Catch::Approx(0.017));
                CHECK(video_player.next_frame(now + 40ms)->timestamp() == Catch::Approx(0.033));
                CHECK(video_player.next_frame(now + 60ms)->timestamp() == Catch::Approx(0.050));

                // skip to the end
                video_player.jump_forward(now + 70ms);  // --> jump 5.0

                CHECK(video_player.next_frame(now + 80ms)->timestamp() == Catch::Approx(4.900));
            }
        }
    }
}

}  // namespace video_trimmer::video_player
