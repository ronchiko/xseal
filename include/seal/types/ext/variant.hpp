#pragma once

#include "seal/types/result.hpp"

namespace seal {
	
	/**
	   Tries to get a value outside of the variant.
	  
	   \param variant
	   \return 
	 */
	template<typename AlternateT, typename... AlternatesT>
	constexpr result<AlternateT&> try_get(std::variant<AlternatesT...>& variant)
	{
		if (!std::holds_alternative<AlternateT>(variant)) {
			return seal::failure("Variant holds expected alternative");
		}

		return std::get<AlternateT>(variant);
	}

	template<typename AlternateT, typename... AlternatesT>
	constexpr result<const AlternateT&> try_get(const std::variant<AlternatesT...>& variant)
	{
		if(!std::holds_alternative<AlternateT>(variant)) {
			return seal::failure("Variant holds expected alternative");
		}

		return std::get<AlternateT>(variant);
	}
}
