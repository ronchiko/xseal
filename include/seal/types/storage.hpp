#pragma once

#include <vector>

#include "seal/types.hpp"
#include "seal/types/paged_storage.hpp"

namespace seal {
	
	namespace detail {
		/**
		   Function to guess the page size of storage using the size of one element in it.
		  
		   \param size: The size of an element in the storage.
		 */
		constexpr size_t guess_page_size(size_t size) {
			constexpr auto DESIRED_ELEMENTS_IN_QUEUE = 20;	// A full page should have 20 elements int
			constexpr auto MINIMUM_PAGE_SIZE = 4096;		// 4KB of storage.

			auto desired = std::bit_ceil(size * DESIRED_ELEMENTS_IN_QUEUE);
			
			if(desired <= MINIMUM_PAGE_SIZE) {
				return MINIMUM_PAGE_SIZE;
			}

			return desired;
		}
	 }
	/**
	   Generic inplace storage, gurantees the addresses of items there wont change under the hood.

	   \tparam StoreT: The value to store in the storage.
	 */
	template<typename StoreT>
	using storage = seal::paged_storage<detail::guess_page_size(sizeof(StoreT)), StoreT>;
}
