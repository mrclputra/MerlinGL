#pragma once

#include "mepch.h"
#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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
		void log(const char* file, int line, const char* func, const char* level, const std::string& fmt, const Args&... args) {
			std::ostringstream oss;
			oss << "[" << getTimestamp() << "]"
				<< "[" << name << "]"
				<< "[" << level << "]"
				<< " " << file << ":" << line << " (" << func << ")"
				<< " | "
				<< format(fmt, args...)
				<< "\n";

			std::string output = oss.str();
			std::lock_guard<std::mutex> lock(mtx);
			std::cout << output;

			if (m_Callback) m_Callback(output);
		}

	public:
		static Logger& getCoreLogger();
		static Logger& getClientLogger();

		template <typename... Args>
		void info(const char* file, int line, const char* func, const std::string& fmt, const Args&... args) {
			log(file, line, func, "Info", fmt, args...);
		}

		template <typename... Args>
		void warning(const char* file, int line, const char* func, const std::string& fmt, const Args&... args) {
			log(file, line, func, "Warn", fmt, args...);
		}

		template <typename... Args>
		void error(const char* file, int line, const char* func, const std::string& fmt, const Args&... args) {
			log(file, line, func, "Error", fmt, args...);
		}

		// todo: this may not be the best approach given my engine architecture
		//	will need to look into alternative methods of passing text into the internal console in the future
		//	but i think this is sufficient for now, and we can have multiple console instances
		using LogFn = std::function<void(const std::string&)>;
		void SetCallback(LogFn cb) { m_Callback = cb; }
	private:
		LogFn m_Callback = nullptr;
	};

	extern MERLIN_API Logger& coreLogger;
	extern MERLIN_API Logger& clientLogger;
};

#include <filesystem>

#define LOG_FILE std::filesystem::path(__FILE__).filename().string().c_str()

// engine logger
#define MERLIN_CORE_INFO(...)   ::Merlin::coreLogger.info(LOG_FILE, __LINE__, __FUNCTION__, __VA_ARGS__)
#define MERLIN_CORE_WARN(...)   ::Merlin::coreLogger.warning(LOG_FILE, __LINE__, __FUNCTION__, __VA_ARGS__)
#define MERLIN_CORE_ERROR(...)  ::Merlin::coreLogger.error(LOG_FILE, __LINE__, __FUNCTION__, __VA_ARGS__)

// client logger
#define MERLIN_INFO(...)		::Merlin::clientLogger.info(LOG_FILE, __LINE__, __FUNCTION__, __VA_ARGS__)
#define MERLIN_WARN(...)        ::Merlin::clientLogger.warning(LOG_FILE, __LINE__, __FUNCTION__, __VA_ARGS__)
#define MERLIN_ERROR(...)       ::Merlin::clientLogger.error(LOG_FILE, __LINE__, __FUNCTION__, __VA_ARGS__)