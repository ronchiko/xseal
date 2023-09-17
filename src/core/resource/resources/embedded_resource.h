#pragma once

#include "seal/api/resource.hpp"

namespace seal {
	class embedded_resource final : public resource_interface
	{
	public:
		inline embedded_resource(const void *data, size_t size)
			: m_Data(reinterpret_cast<const u8*>(data))
			, m_Size(size)
			, m_CurrentRead(0)
		{}

		virtual ~embedded_resource() override = default;

		virtual std::span<const u8> load_all() override;

		virtual u32 load(std::span<u8> buffer) override;

	private:
		const u8 *m_Data;
		size_t m_Size;
		size_t m_CurrentRead;
	};
}
