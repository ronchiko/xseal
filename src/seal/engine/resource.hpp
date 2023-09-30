#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "seal/api/resource.hpp"
#include "seal/log/log.hpp"

namespace seal {

	template<typename RealtimeObjectT>
	struct resource_loader;

	class resource_collection
	{
	public:
		struct resource_collection_object
		{
			virtual ~resource_collection_object() = default;
		};

		/**
			Attaches a resource to this collection.
		 */
		void attach(std::unique_ptr<resource_collection_object> resource);

	private:
		std::vector<std::unique_ptr<resource_collection_object>> m_Resources;
	};

	/**
	   A reference for an object that is created from a resource.
	 */
	template<typename RealtimeObjectT>
	class resource_ref final : public resource_collection::resource_collection_object
	{
	public:
		using object_t = RealtimeObjectT;

		/**
		   Creates a new resource reference from a path.

		   \param path: The path to the resource.
		 */
		constexpr resource_ref(std::string path);

		constexpr resource_ref(const char *path)
			: resource_ref(std::string(path))
		{}

		/**
		   Loads the referenced object.
		   Note: Calling this method when the reference is valid will cause a reload.
		 */
		RealtimeObjectT& load() const
		{
			auto loaded_resource = load(m_Path);

			m_Instance = std::move(loaded_resource);
			return *m_Instance;
		}

		/**
			Gets a shared pointer to the realtime object held by this resource.

			Use this method when you want to keep the object alive.
		*/
		std::shared_ptr<RealtimeObjectT> co_own() const
		{
			if(nullptr == m_Instance || 0 == m_Instance.use_count()) {
				load();
			}

			return m_Instance;
		}

		/**
		   Same as load, but if the object is currently loaded, uses the cached object.
		 */
		RealtimeObjectT& acquire() const
		{
			if(nullptr == m_Instance || 0 == m_Instance.use_count()) {
				return load();
			}

			return *m_Instance;
		}

	private:
		std::string m_Path;
		mutable std::shared_ptr<RealtimeObjectT> m_Instance;

		static std::shared_ptr<RealtimeObjectT> load(const std::string& path);
	};
}

template<typename RealtimeT>
constexpr seal::resource_ref<RealtimeT>::resource_ref(std::string path)
	: m_Path(std::move(path))
	, m_Instance(nullptr)
{}

template<typename RealtimeT>
std::shared_ptr<RealtimeT> seal::resource_ref<RealtimeT>::load(const std::string& path)
{
	static std::unordered_map<std::string, std::weak_ptr<object_t>> s_Loaded{};

	auto weak_ref = s_Loaded.find(path);
	if(s_Loaded.end() == weak_ref || weak_ref->second.expired()) {
		// We reached this point if our resource isn't currently in memory, so we need to load the
		// resource from scratch.

		seal::log::info("Loading resource {}", path);

		// Load the resource as a binary.
		auto resource = seal::load_resource(path);

		// Now interpret that blob as something using a resource_loader implementation.
		auto parsed_resource = resource_loader<object_t>::load(resource);
		static_assert(std::is_same_v<decltype(parsed_resource), std::shared_ptr<object_t>>,
					  "resource_loader<RealtimeT>::load doesn't return a std::shared_ptr<RealtimeT>");

		s_Loaded[path] = parsed_resource;
		return parsed_resource;
	}

	// If we already have a second one we just lock it.
	return weak_ref->second.lock();
}
