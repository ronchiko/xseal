#include "seal/log/log.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "seal/types/result.hpp"

#include "conosle_color.inl"
#ifdef SEAL_WINDOWS
#include "color.windows.inl"
#else
#include "color.default.inl"
#endif


namespace seal::log {
	static std::fstream g_Logfile;

	constexpr const char *SEVERITY_PREFIXES[] = {
		"[DEBUG]",
		"[INFO]",
		"[WARNING]",
		"[ERROR]",
	};

	constexpr console_color SEVERITY_COLORS[] = {
		console_color::Green,
		console_color::Blue,
		console_color::Yellow,
		console_color::Red,
	};

	seal::result<void> initialize()
	{
		g_Logfile = std::fstream(std::filesystem::current_path() / "seal.log", std::ios::out);

		return {};
	}

	/**
	   Write a colored prefix to the console.
	  
	   \param severity: The severity index
	 */
	static void write_log_prefix(u32 severity)
	{
		color_context ctx(SEVERITY_COLORS[severity]);
		std::cout << SEVERITY_PREFIXES[severity];
	}

	void write_log(Severity severity, const char *message)
	{
		const auto severity_index = static_cast<std::uint32_t>(severity);

		// Only log to console when running debug mode
#ifdef SEAL_DEBUG
		write_log_prefix(severity_index);
		std::cout << ": " << message << std::endl;
#endif
		g_Logfile << SEVERITY_PREFIXES[severity_index] << ": " << message << std::endl;
	}

#ifdef SEAL_DEBUG
	void debug(const char *message)
	{
		write_log(Severity::Debug, message);
	}
#endif
}