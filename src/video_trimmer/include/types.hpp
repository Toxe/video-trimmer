#pragma once

namespace video_trimmer {

struct Position {
    int x, y;
};

struct Size {
    int width, height;
};

inline bool operator==(const Size& lhs, const Size& rhs) { return lhs.width == rhs.width && lhs.height == rhs.height; }
inline bool operator!=(const Size& lhs, const Size& rhs) { return !(lhs == rhs); }

}  // namespace video_trimmer
