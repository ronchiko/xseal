#pragma once

#include "seal/api/resource.hpp"

namespace seal {
	class embedded_resource final : public resource_interface
	{
	public:
		explicit embedded_resource(const void *data, const size_t size)
			: m_Data(static_cast<const u8*>(data))
			, m_Size(size)
			, m_CurrentRead(0)
		{}

		virtual std::span<const u8> load_all() override;

		virtual std::span<const u8> load(u32 bytes_to_read) override;

	private:
		const u8 *m_Data;
		size_t m_Size;
		size_t m_CurrentRead;
	};
}
