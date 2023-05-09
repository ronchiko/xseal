#pragma once

#include <memory>

#include <gtest/gtest.h>

namespace seal::ut {

	/**
	   Type to help count allocation and destruction for unit tests.
	 */
	struct destory_t
	{
		/// The amount of allocations
		inline static size_t alloc_count = 0;
		/// The amount of destructions
		inline static size_t destroy_count = 0;

		// Allocates a new data_t
		static std::unique_ptr<int, destory_t> alloc(int value)
		{
			++alloc_count;
			return std::unique_ptr<int, destory_t>(new int(value));
		}

		/// Resets the destroy_t statictics
		static void reset()
		{
			alloc_count = 0;
			destroy_count = 0;
		}

		void operator()(int *address)
		{
			if(nullptr != address) {
				++destroy_count;
				delete address;
			}
		}
	};

	using data_t = std::unique_ptr<int, destory_t>;
}
