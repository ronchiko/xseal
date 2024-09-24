#pragma once

#include <string>

#include <fmt/format.h>

#include "seal/types/failure.hpp"
#include "seal/debug.h"
#include "seal/defenitions.h"


namespace seal::log {
	enum class severity
	{
		Debug,
		Info,
		Warning,
		Error
	};

	/**
		Initializes the logging system for XSeal.
	 */
	void initialize();

	/**
		Writes a log message.
	 
		\param severity: The severity of the message
		\param message: The message to log
	 */
	void write_log(severity severity, const char *message);

	template<typename... Args>
	void info(const char *fmt, Args&&...args)
	{
		write_log(severity::Info, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
	}

	template<typename... Args>
	void warning(const char *fmt, Args&&...args)
	{
		write_log(severity::Warning, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
	}

	template<typename... Args>
	void error(const char *fmt, Args&&...args)
	{
		write_log(severity::Error, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
	}

#ifdef SEAL_DEBUG
	void debug(const char *message);

	template<typename... Args>
	void debug(const char *fmt, Args&&...args)
	{
		write_log(severity::Debug, fmt::vformat(fmt, fmt::make_format_args(args...)).c_str());
	}
#else
	// In release mode no do nothing
	seal_force_inline consteval void debug(...) {}
#endif
}
