#include <Merlin/Logger.h>

namespace Merlin {
	Logger& logger = Logger::instance();
	Logger& Logger::instance() {
		static Logger inst;
		return inst;
	}

    std::string Logger::getTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::tm tm{};
#ifdef _WIN32
        localtime_s(&tm, &time);
#else
        localtime_r(&time, &tm);
#endif
        std::ostringstream oss;
        oss << std::put_time(&tm, "%M:%S.") << std::setfill('0') << std::setw(3) << ms.count() << "ms";
        return oss.str();
    }

    // GLM overloads
    std::string Logger::toString(const glm::vec2& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::vec3& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::vec4& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::mat3& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::mat4& v) { return glm::to_string(v); }
}