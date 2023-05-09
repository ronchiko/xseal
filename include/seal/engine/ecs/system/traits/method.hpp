#pragma once

#include <tuple>
#include <type_traits>

namespace seal::ecs::detail {
	template<typename MemberMethodT>
	struct method_traits;

	template<typename HostT, typename ReturnT, typename... ArgumentsT>
	struct method_traits<ReturnT(HostT::*)(ArgumentsT...)>
	{
	public:
		using return_t = ReturnT;
		using host_t = HostT;
		using tuple_t = std::tuple<ArgumentsT...>;
	};
}
