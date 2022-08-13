#pragma once

namespace video_trimmer {

struct Position {
    int x = 0;
    int y = 0;
};

struct Size {
    int width = 0;
    int height = 0;
};

inline bool operator==(const Size& lhs, const Size& rhs) { return lhs.width == rhs.width && lhs.height == rhs.height; }
inline bool operator!=(const Size& lhs, const Size& rhs) { return !(lhs == rhs); }

}  // namespace video_trimmer
