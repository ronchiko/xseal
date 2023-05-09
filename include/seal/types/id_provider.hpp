#pragma once

#include "seal/types.hpp"
#include "seal/panic.h"

namespace seal {
	/**
	   A class used to generate id's for objects.

	   \note: Ids are unique to each provider, and not globaly unique.
	 */
	class id_provider
	{
	public:	
		constexpr id_provider() noexcept;
	
		/**
		   Makes a new unique id.
		 */
		inline id make_id();

	private:
		id m_Id;
	};
}

constexpr seal::id_provider::id_provider() noexcept
	: m_Id(INVALID_ID + 1)
{}

seal::id seal::id_provider::make_id() {
	// If the id is INVALID_ID it means we got an overflow and used too many ids
	if(INVALID_ID == m_Id) {
		seal_panic("Id limit reached!");
	}

	return m_Id++;
}
