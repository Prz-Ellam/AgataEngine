#ifndef AGATA_LOG_H
#define AGATA_LOG_H

#include <string>

	enum LogType {
		Trace,
		Debug,
		Info,
		Warn,
		Error,
		Fatal
	};

	class Log {
	public:
		Log() = delete;
		~Log() = delete;

		template <typename... Args>
		static void trace(const std::string& message, Args... args);

		template <typename... Args>
		static void debug(const std::string& message, Args... args);

		template <typename... Args>
		static void info(const std::string& message, Args... args);

		template <typename... Args>
		static void warn(const std::string& message, Args... args);

		template <typename... Args>
		static void error(const std::string& message, Args... args);

		template <typename... Args>
		static void fatal(const std::string& message, Args... args);

	private:
		static LogType logType;
	};

	template <typename... Args>
	void Log::trace(const std::string& message, Args... args) {

		printf("\x1B[32m[Trace]\033[0m\t\t");
		printf(message.c_str(), args...);
		printf("\n");

	}

	template <typename... Args>
	void Log::debug(const std::string& message, Args... args) {

		printf("\x1B[36m[Debug]\033[0m\t\t");
		printf(message.c_str(), args...);
		printf("\n");

	}

	template <typename... Args>
	void Log::info(const std::string& message, Args... args) {

		printf("\x1B[32m[Info]\033[0m\t\t");
		printf(message.c_str(), args...);
		printf("\n");

	}

	template <typename... Args>
	void Log::warn(const std::string& message, Args... args) {

		printf("\x1B[93m[Warn]\033[0m\t\t");
		printf(message.c_str(), args...);
		printf("\n");

	}

	template <typename... Args>
	void Log::error(const std::string& message, Args... args) {

		printf("\x1B[31m[Error]\033[0m\t\t");
		printf(message.c_str(), args...);
		printf("\n");

	}

	template <typename... Args>
	void Log::fatal(const std::string& message, Args... args) {

		printf("\x1B[31m[Fatal]\033[0m\t\t");
		printf(message.c_str(), args...);
		printf("\n");

	}

#endif