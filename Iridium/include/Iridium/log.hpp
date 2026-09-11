#ifndef IRIDIUM_LOG_HPP_
#define IRIDIUM_LOG_HPP_

/// Comment out to disable logging
#define IRIDIUM_ENABLE_LOGGING
#define IRIDIUM_LOG_TO_FILE 0

#ifdef IRIDIUM_ENABLE_LOGGING
	/// @brief Logs info messages to output stream
	#define LOG_INFO(msg) ir::log::info(msg)
	/// @brief Logs warning messages to output stream
	#define LOG_WARN(msg) ir::log::warn(msg)
	/// @brief Logs error messages to output stream
	#define LOG_ERROR(msg) ir::log::error(msg)
#else
	#define LOG_INFO(msg) ((void)0)
	#define LOG_WARN(msg) ((void)0)
	#define LOG_ERROR(msg) ((void)0)
#endif // IRIDIUM_ENABLE_LOGGING

#include <string_view>

namespace ir::log {
	void startSession();

	void info(std::string_view msg); ///< @brief Logs info messages to output stream
	void warn(std::string_view msg); ///< @brief Logs warning messages to output stream
	void error(std::string_view msg); ///< @brief Logs error messages to output stream

	void endSession();
}

#endif // IRIDIUM_LOG_HPP_