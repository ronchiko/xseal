#pragma once

#include <algorithm>
#include <string>
#include <unordered_map>

#include "seal/types.hpp"
#include "seal/types/buffer.hpp"

namespace seal {

	struct dyanmic_struct_member
	{
		u32 offset;
		u32 size;
	};

	struct dynamic_struct_layout
	{
		std::unordered_map<std::string, dyanmic_struct_member> layout;

		inline u32 expected_size() const
		{
			if(layout.empty()) {
				return 0;
			}

			const auto max = std::max_element(layout.begin(),
											  layout.end(),
											  [](const auto& first, const auto& second) {
												  return first.second.offset > second.second.offset;
											  });

			return max->second.offset + max->second.size;
		}
	};

	class dynamic_struct
	{
	public:


	private:
		dynamic_struct_layout m_layout;
		buffer<u8> m_bytes;
	};
}
