#include "seal/log/log.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace seal::log {
	static std::fstream g_Logfile;

	constexpr const char *SEVERITY_PREFIXES[] = {
		"[DEBUG]",
		"[INFO]",
		"[WARNING]",
		"[ERROR]",
	};

	seal::result<void> initialize() {
		g_Logfile = std::fstream(std::filesystem::current_path() / "seal.log", std::ios::out);
		
		return {};
	}

	void log(Severity severity, const char *message)
	{
		const auto severity_index = static_cast<std::uint32_t>(severity);

		// Only log to console when running debug mode
#ifdef SEAL_DEBUG
		std::cout << SEVERITY_PREFIXES[severity_index] << ": " << message << std::endl;
#endif
		g_Logfile << SEVERITY_PREFIXES[severity_index] << ": " << message << std::endl;
	}

#ifdef SEAL_DEBUG
	void debug(const char *message)
	{
		log(Severity::Debug, message);
	}
#endif
}