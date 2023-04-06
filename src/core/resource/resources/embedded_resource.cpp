#include "embedded_resource.h"

#include <algorithm>

namespace seal {
	result<std::span<const u8>> embedded_resource::load_all()
	{
		if (m_CurrentRead >= m_Size) {
			return seal::fail<failure::EndReached>("Finished reading the resource.");
		}

		m_CurrentRead = m_Size;
		return std::span{ m_Data, m_Size };
	}

	result<u32> embedded_resource::load(std::span<u8> buffer) {
		const size_t bytes_to_read = std::min(m_Size - m_CurrentRead, buffer.size());

		const auto end_iter = std::next(buffer.begin(), bytes_to_read); 
		std::transform(buffer.begin(), end_iter, buffer.begin(), [self = this](const u8& item) {
			return self->m_Data[self->m_CurrentRead++];
		});

		return static_cast<u32>(bytes_to_read);
	}
}
