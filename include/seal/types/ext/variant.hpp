#pragma once

#include "seal/types/failure.hpp"

namespace seal {
	
	/**
	   Tries to get a value outside of the variant.
	  
	   \param variant
	   \return 
	 */
	template<typename AlternateT, typename... AlternatesT>
	constexpr AlternateT& try_get(std::variant<AlternatesT...>& variant)
	{
		if (!std::holds_alternative<AlternateT>(variant)) {
			throw seal::failure("Variant holds unexpected alternative");
		}

		return std::get<AlternateT>(variant);
	}

	template<typename AlternateT, typename... AlternatesT>
	constexpr const AlternateT& try_get(const std::variant<AlternatesT...>& variant)
	{
		if(!std::holds_alternative<AlternateT>(variant)) {
			throw seal::failure("Variant holds unexpected alternative");
		}

		return std::get<AlternateT>(variant);
	}
}
