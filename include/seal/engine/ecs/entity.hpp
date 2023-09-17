#pragma once

#include <entt/entt.hpp>

#include "seal/types.hpp"
#include "seal/types/failure.hpp"

namespace seal {

	using entity_id = id;

	class entity
	{
	public:
		/**
		   Creates a new entity.
		 */
		static entity create();

		/**
		   Finds an entity by its id.
		  
		   \param id: The id of the entity to look for.
		 */
		static entity find(entity_id id);

		/**
			Destroys this entity.

			\note: After using this function you can no longer use this entity object.
		 */ 
		void erase(void);

		/**
		   Adds a component to this entity.

		  \tparam ComponentT: The type of component to add.
		 */
		template<typename ComponentT, typename... Args>
		ComponentT& add(Args&&...args);

		/**
			Erases components from this entity.
		 */
		template<typename... Components>
		void erase() const;

		/**
			Gets a component from this entity.
		*/
		template<typename... Components>
		std::tuple<std::remove_cvref_t<Components>&...> get();
		
		/**
		   The Id of this entity.
		 */
		[[nodiscard]]
		constexpr entity_id id() const;

		inline static entt::basic_registry<entity_id> g_Registry;

	private:
		explicit entity(entity_id id);

		entity_id m_Id;
	};
}

template<typename ComponentT, typename... Args>
ComponentT& seal::entity::add(Args&&... args)
{
	return g_Registry.emplace<ComponentT>(m_Id, args...);
}

template<typename... Components>
void seal::entity::erase() const
{
	g_Registry.remove<Components...>(m_Id);
}

template<typename... Components>
std::tuple<std::remove_cvref_t<Components>&...> seal::entity::get()
{
	// Ensure the entity has all the components.
	if (!g_Registry.all_of<std::remove_cvref_t<Components>...>(m_Id)) {
		throw seal::failure("Entity doesn't have all the components.");
	}

	return std::tie(g_Registry.get<Components>(m_Id)...);
}

constexpr seal::entity_id seal::entity::id() const {
	return m_Id;
}
