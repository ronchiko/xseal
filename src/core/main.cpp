#include <iostream>
#include <vector>

#include "seal/log/log.hpp"
#include "seal/types/result.hpp"

/**
 * Initialize the XSeal engine.
 */
seal::result<void> initialize_engine()
{
	seal_verify_result(seal::log::initialize());
	seal::log::debug("Starting to initialize engine");

	return {};
}

int main()
{
	initialize_engine().expect("Failed to initialize engine");
	
#ifdef SEAL_WEBGL
	seal::log::info("Running in webGL");
#endif


	return 0;
}