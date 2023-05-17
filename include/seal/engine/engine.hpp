#pragma once

#include "seal/types/result.hpp"

namespace seal {
	
	class engine
	{
	public:
		constexpr engine() = default;
		/*
			Creates a new engine object, initializing whatever is nessecary when called.
		 */
		static result<engine> create();

		/**
		  Invokes an engine tick.
		 */ 
		void tick(float deltaTime);
	};
}
