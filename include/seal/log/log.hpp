#pragma once

#include <string>

#include <fmt/format.h>

#include "seal/defenitions.h"
#include "seal/types.hpp"
#include "seal/types/result.hpp"


namespace seal {

	namespace log {
		enum class Severity
		{
			Debug,
			Info,
			Warning,
			Error
		};

		/**
		 * @brief Initializes the logging system for XSeal
		 */
		seal::result<void> initialize();

		/**
		 * @brief Writes a log message
		 *
		 * @param severity: The severity of the message
		 * @param message: The message to log
		 */
		void log(Severity severity, const char *message);

		template<typename... Args>
		void info(const char *fmt, Args&&...args)
		{
			log(Severity::Info, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
		}

		template<typename... Args>
		void warning(const char *fmt, Args&&...args)
		{
			log(Severity::Warning, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
		}

		template<typename... Args>
		void error(const char *fmt, Args&&...args)
		{
			log(Severity::Error, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
		}

#ifdef SEAL_DEBUG
		void debug(const char *message);

		template<typename... Args>
		void debug(const char *fmt, Args&&...args)
		{
			log(Severity::Debug, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
		}
#else
		// In release mode no do nothing
		seal_force_inline consteval void debug(...) {}
#endif
	}
}