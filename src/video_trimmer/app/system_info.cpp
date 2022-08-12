#include "system_info.hpp"

#include "SDL2/SDL_endian.h"
#include "SDL2/SDL_platform.h"
#include "fmt/core.h"

#include "../logger/logger.hpp"

namespace video_trimmer::app {

void show_system_info()
{
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
    const char* endian = "little";
#else
    const char* endian = "big";
#endif

    logger::log_info(fmt::format("{}, {}-endian", SDL_GetPlatform(), endian));

#if defined(_MSC_VER) && !defined(__clang_version__)
    logger::log_info(fmt::format("MSVC {}, C++ {}", _MSC_VER, _MSVC_LANG));
#endif

#if defined(__clang_version__) && !defined(_MSC_VER)
    logger::log_info(fmt::format("Clang {}, C++ {}", __clang_version__, __cplusplus));
#ifdef _LIBCPP_VERSION
    logger::log_info(fmt::format("libc++ {}", _LIBCPP_VERSION));
#endif
#ifdef _GLIBCXX_RELEASE
    logger::log_info(fmt::format("libstdc++ {}", _GLIBCXX_RELEASE));
#endif
#endif

#if defined(__clang_version__) && defined(_MSC_VER)
    logger::log_info(fmt::format("Clang-cl {}, C++ {}, MSVC {}", __clang_version__, __cplusplus, _MSC_VER));
#endif

#if defined(__GNUC__) && !defined(__clang_version__)
    logger::log_info(fmt::format("GCC {}, libstdc++ {}, C++ {}", __GNUC__, _GLIBCXX_RELEASE, __cplusplus));
#endif
}

}  // namespace video_trimmer::app
