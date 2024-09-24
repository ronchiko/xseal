#include "seal/api/resource.hpp"
#include "seal/defenitions.h"
#include "seal/panic.h"
#include "seal/types/id_provider.hpp"

namespace seal {
	namespace {
		id_provider g_ResourceIdProvider;
	}

	unique_resource::unique_resource(std::string path, resource_interface *resource)
		: m_Path(std::move(path))
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

	std::span<const u8> unique_resource::load_all() const
	{
		seal_assert(nullptr != m_Resource, "Usage of null resource!");

		return m_Resource->load_all();
	}

	std::span<const u8> unique_resource::load(const u32 bytes) const
	{
		seal_assert(nullptr != m_Resource, "Usage of null resource!");

		return m_Resource->load(bytes);
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
