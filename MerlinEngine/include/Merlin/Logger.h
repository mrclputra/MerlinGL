#pragma once

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <chrono>
#include <iomanip>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "Merlin/Config.h"

namespace Merlin {

	class MERLIN_API Logger {
	private:
		std::mutex mtx;

		Logger() = default;
		~Logger() = default;

		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;

		template<typename T>
		static std::string toString(const T& v) {
			std::ostringstream oss;
			oss << v;
			return oss.str();
		}

		static std::string toString(const glm::vec2& v);
		static std::string toString(const glm::vec3& v);
		static std::string toString(const glm::vec4& v);
		static std::string toString(const glm::mat3& v);
		static std::string toString(const glm::mat4& v);

		static std::string getTimestamp();

		template<typename... Args>
		void log(
			const char* level,
			const Args&... args
		) {
			std::ostringstream oss;
			oss << "[" << getTimestamp() << "]"
				<< "[" << level << "]"
				<< " | ";

			(void)std::initializer_list<int>{
				(oss << toString(args), 0)...
			};

			oss << "\n";

			std::lock_guard<std::mutex> lock(mtx);
			std::cout << oss.str();
			std::cout.flush();
		}

	public:
		static Logger& instance();

		template <typename... Args>
		void info(const Args&... args) {
			log("INFO", args...);
		}

		template <typename... Args>
		void warning(const Args&... args) {
			log("WARN", args...);
		}

		template <typename... Args>
		void error(const Args&... args) {
			log("ERROR", args...);
		}
	};

	extern MERLIN_API Logger& logger;
};