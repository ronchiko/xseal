#include "seal/api/resource.hpp"

#include "seal/panic.h"
#include "seal/types/id_provider.hpp"

namespace seal {
	static id_provider g_ResourceIdProvider;

	unique_resource::unique_resource(std::string path, resource_interface *resource)
		: m_Path(path)
		, m_Id(g_ResourceIdProvider.make_id())
		, m_Resource(resource)
	{}

	unique_resource& unique_resource::operator=(unique_resource&& other) noexcept
	{
		release();

		std::exchange(m_Id, other.m_Id);
		std::exchange(m_Path, other.m_Path);
		std::exchange(m_Resource, other.m_Resource);

		return *this;
	}

	unique_resource::~unique_resource() noexcept
	{
		seal_mute_exceptions({ release(); });
	}

	result<std::span<const u8>> unique_resource::load_all()
	{
		seal_assert(nullptr != m_Resource, "Usage of null resource!");

		return m_Resource->load_all();
	}

	result<u32> unique_resource::load(std::span<u8> span)
	{
		seal_assert(nullptr != m_Resource, "Usage of null resource!");

		return m_Resource->load(span);
	}

	void unique_resource::release() noexcept
	{
		seal_mute_exceptions({
			if(nullptr != m_Resource) {
				delete m_Resource;
			}

			reset();
		});
	}
}
