#pragma once

namespace seal {
	
	class engine
	{
	public:
		constexpr engine() = default;
		/*
			Creates a new engine object, initializing whatever is necessary when called.
		 */
		static void create();

		/**
		  Invokes an engine tick.
		 */
		static void tick(float delta_time);
	
		/**
			Gracefully stops the engine
		 */
		static void stop();
	};
}
