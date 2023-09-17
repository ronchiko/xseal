#include "embedded_resource.h"

#include <algorithm>

#include "seal/types/failure.hpp"

namespace seal {
	std::span<const u8> embedded_resource::load_all()
	{
		if (m_CurrentRead >= m_Size) {
			throw seal::failure("Cant read any more of the resource");
		}

		m_CurrentRead = m_Size;
		return std::span{ m_Data, m_Size };
	}

	u32 embedded_resource::load(std::span<u8> buffer) {
		const size_t bytes_to_read = std::min(m_Size - m_CurrentRead, buffer.size());

		const auto end_iterator = std::next(buffer.begin(), bytes_to_read); 
		std::transform(buffer.begin(), end_iterator, buffer.begin(), [self = this](const u8& item) {
			return self->m_Data[self->m_CurrentRead++];
		});

		return static_cast<u32>(bytes_to_read);
	}
}
