#pragma once

#include <locale>
#include <unordered_map>

#include "failure.hpp"
#include "seal/types/traits/template.hpp"

namespace seal {
	namespace detail {
		/**
			Given an unordered map creates a new one where each element is flipped.
		 */
		template<typename Ty, typename Tx>
		auto flip_elements(const std::unordered_map<Ty, Tx>& input)
		{
			std::unordered_map<Tx, Ty> swapped;
			for(const auto& [first, second] : input) {
				swapped[second] = first;
			}

			return swapped;
		}
	}

	// clang-format off
	template<typename T>
	concept conversion_table_concept = requires(T instance) {
	   { T::MAPPING } -> seal::template_instance_of<std::unordered_map>;
	};
	// clang-format on

	template<typename LookupT, typename ValueT>
	struct conversion_table;

	/**
		Converts between 2 string.
	 */
	template<typename ValueT, typename LookupT>
	ValueT convert(const LookupT& lookup)
	{
		auto iterator = conversion_table<LookupT, ValueT>::MAPPING.find(lookup);
		if(conversion_table<LookupT, ValueT>::MAPPING.end() == iterator) {
			throw failure("Failed to convert from {} to {}.",
						  typeid(LookupT).name(),
						  typeid(ValueT).name());
		}

		return iterator->second;
	}

	/**
		Converts between a string to its matching value, while making the string lowercase.
	 */
	template<typename ValueT>
	ValueT convert_lowercase(std::string lookup)
	{
		std::ranges::transform(lookup, [](const auto& c) { return std::tolower(c); });
		return convert<ValueT>(lookup);
	}
}

/**
	Creates a conversion table between `lookup_type` to `value_type`.
 */
#define SEAL_CONVERSION_TABLE(lookup_type, value_type, ...)                                        \
	template<>                                                                                     \
	struct ::seal::conversion_table<lookup_type, value_type>                                       \
	{                                                                                              \
		inline static const ::std::unordered_map<lookup_type, value_type> MAPPING = __VA_ARGS__;   \
	}

/**
	Creates a conversion table between `lookup_type` and `value_type` and also creates a mirrored
	version of that table between `value_type` to `lookup_type`.
 */
#define SEAL_BI_CONVERSION_TABLE(lookup_type, value_type, ...)                                     \
	SEAL_CONVERSION_TABLE(lookup_type, value_type, __VA_ARGS__);                                   \
	SEAL_CONVERSION_TABLE(value_type,                                                              \
						  lookup_type,                                                             \
						  ::seal::detail::flip_elements(::seal::conversion_table<lookup_type,      \
																				 value_type>::MAPPING))