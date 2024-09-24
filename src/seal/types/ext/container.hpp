#pragma once

#include "seal/types.hpp"

namespace seal
{
	template<typename ContainerLike>
	auto& warp_view(ContainerLike&& container, i32 index)
	{
		if(index < 0) {
			index = std::size(container) + index;
		}

		return container[index];
	}

}
