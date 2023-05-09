#pragma once

#include <type_traits>

namespace seal {
	
	namespace detail {
		template<size_t Index, typename... List>
		struct skip_to;

		template<size_t Index>
		struct skip_to<Index>
		{
		public:
			using type = type_list<Index>;
		};

		template<size_t Index, typename Current, typename... List>
		struct skip_to<Index, Current, List...>
		{
		private:
			using next = skip_to<Index - 1, List...>;  

		public:
			using type = std::conditional_t<(Index > 0), next, type_list<List...>>;
		};
	}

	template<typename... Types>
	class type_list
	{
	private:
	public:
		template<size_t FirstIndex>
		using trim_front = 
	};
}