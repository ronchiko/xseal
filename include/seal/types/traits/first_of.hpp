#pragma once

#include <type_traits>

namespace seal {
	
	template<typename...>
	struct first_of;

	template<typename First, typename... Rest>
	struct first_of<First, Rest...>
	{
		using type_t = First;
	};

	template<typename... List>
	using first_of_t = typename first_of<List...>::type_t;
}