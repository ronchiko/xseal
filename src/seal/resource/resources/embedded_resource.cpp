#include "embedded_resource.h"

#include "seal/types/failure.hpp"

namespace seal {
	std::span<const u8> embedded_resource::load_all()
	{
		if (m_CurrentRead >= m_Size) {
			throw failure("Cant read any more of the resource");
		}

		m_CurrentRead = m_Size;
		return std::span{ m_Data, m_Size };
	}

	std::span<const u8> embedded_resource::load(u32 bytes_to_read) {
		// Ensure we are inside the bounds.
		if(m_CurrentRead + bytes_to_read >= m_Size)
		{
			bytes_to_read = static_cast<u32>(m_Size - m_CurrentRead);
		}

		const std::span view{m_Data + m_CurrentRead, bytes_to_read};
		m_CurrentRead += bytes_to_read;

		return view;
	}
}
