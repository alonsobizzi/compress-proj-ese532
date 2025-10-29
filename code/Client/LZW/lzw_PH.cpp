#include <cstring>

size_t lzw_PH(const void* source, void* dest, size_t length) {
    if (!source || !dest || length == 0)
        return 1;  // Sad days when nothing
    std::memcpy(dest, source, length);
    return length;
}
