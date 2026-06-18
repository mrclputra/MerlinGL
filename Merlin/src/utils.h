#ifndef MERLINGL_UTILS_H
#define MERLINGL_UTILS_H

namespace utils {

constexpr const char *getPlatformName() {
#if defined(_WIN32)
   return "Windows";
#elif defined(__APPLE__) && defined(__MACH__)
   return "macOS";
#elif defined(__linux__)
   return "Linux";
#elif defined(__FreeBSD__)
   return "FreeBSD";
#else
   return "Unknown Platform";
#endif
}

}; // namespace utils

#endif // MERLINGL_UTILS_H
