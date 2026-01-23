#include <Merlin/Logger.h>

namespace Merlin {
    static const auto startTime = std::chrono::steady_clock::now();

	Logger& logger = Logger::instance();
	Logger& Logger::instance() {
		static Logger inst;
		return inst;
	}

    std::string Logger::getTimestamp() {
        auto now = std::chrono::steady_clock::now();
        auto duration = now - startTime;

        auto total_ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

        uint64_t seconds = (total_ms / 1000) % 60;
        uint64_t minutes = (total_ms / (1000 * 60));
        uint64_t ms = total_ms % 1000;

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << seconds << "."
            << std::setfill('0') << std::setw(3) << ms << "s";

        return oss.str();
    }

    // GLM overloads
    std::string Logger::toString(const glm::vec2& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::vec3& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::vec4& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::mat3& v) { return glm::to_string(v); }
    std::string Logger::toString(const glm::mat4& v) { return glm::to_string(v); }
}