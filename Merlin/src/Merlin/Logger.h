#pragma once

#include "mepch.h"
#include <iomanip>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Merlin/Core.h"

namespace Merlin {

	class MERLIN_API Logger {
	private:
		std::mutex mtx;
		std::string name;

		Logger(const std::string& name) : name(name) {};
		~Logger() = default;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		template<typename T>
		static std::string toString(const T& v) {
			std::ostringstream oss;
			oss << v;
			return oss.str();
		}

		// glm overloads
		static std::string toString(const glm::vec2& v);
		static std::string toString(const glm::vec3& v);
		static std::string toString(const glm::vec4& v);
		static std::string toString(const glm::mat3& v);
		static std::string toString(const glm::mat4& v);

		static std::string getTimestamp();

		template<typename... Args>
		std::string format(const std::string& fmt, const Args&... args) {
			std::vector<std::string> argStrings = { toString(args)... };
			std::string result = fmt;

			for (size_t i = 0; i < argStrings.size(); ++i) {
				std::string placeholder = "{" + std::to_string(i) + "}";
				size_t pos = 0;
				while ((pos = result.find(placeholder, pos)) != std::string::npos) {
					result.replace(pos, placeholder.length(), argStrings[i]);
					pos += argStrings[i].length();
				}
			}

			return result;
		}

		template<typename... Args>
		void log(const char* level, const std::string& fmt, const Args&... args) {
			std::ostringstream oss;
			oss << "[" << getTimestamp() << "]"
				<< "[" << name << "]"
				<< "[" << level << "]"
				<< " | "
				<< format(fmt, args...)
				<< "\n";

			std::lock_guard<std::mutex> lock(mtx);
			std::cout << oss.str();
			std::cout.flush();
		}

	public:
		static Logger& getCoreLogger();
		static Logger& getClientLogger();

		template <typename... Args>
		void info(const std::string& fmt, const Args&... args) {
			log("Info", fmt, args...);
		}

		template <typename... Args>
		void warning(const std::string& fmt, const Args&... args) {
			log("Warn", fmt, args...);
		}

		template <typename... Args>
		void error(const std::string& fmt, const Args&... args) {
			log("Error", fmt, args...);
		}
	};

	extern MERLIN_API Logger& coreLogger;
	extern MERLIN_API Logger& clientLogger;
};


// engine logger
#define MERLIN_CORE_INFO(...)		::Merlin::coreLogger.info(__VA_ARGS__)
#define MERLIN_CORE_WARN(...)		::Merlin::coreLogger.warning(__VA_ARGS__)
#define MERLIN_CORE_ERROR(...)		::Merlin::coreLogger.error(__VA_ARGS__)
// client logger
#define MERLIN_INFO(...)		::Merlin::clientLogger.info(__VA_ARGS__)
#define MERLIN_WARN(...)		::Merlin::clientLogger.warning(__VA_ARGS__)
#define MERLIN_ERROR(...)		::Merlin::clientLogger.error(__VA_ARGS__)