#include "seal/engine/ecs/entity.hpp"

seal::entity seal::entity::create()
{
	auto entity_id = g_Registry.create();

	return entity{ entity_id };
}

seal::entity seal::entity::find(entity_id id)
{
	return entity{ id };
}

void seal::entity::erase(void)
{
	g_Registry.destroy(m_Id);
}

seal::entity::entity(entity_id id)
	: m_Id(id)
{}
