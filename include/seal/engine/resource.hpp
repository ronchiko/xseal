#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "seal/api/resource.hpp"

namespace seal {

	template<typename RealtimeObjectT>
	struct resource_loader;

	/**
	   A reference for an object that is created from a resource.
	 */
	template<typename RealtimeObjectT>
	class resource_ref
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
		   Note: Calling this method when the refernce is valid will cause a reload.
		 */
		result<RealtimeObjectT&> load() const
		{
			auto loaded_resource = load(m_Path);
			seal_verify_result(loaded_resource);

			m_Instance = std::move(*loaded_resource);
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
		result<RealtimeObjectT&> acquire() const
		{
			if(nullptr == m_Instance || 0 == m_Instance.use_count()) {
				return load();
			}

			return *m_Instance;
		}

	private:
		std::string m_Path;
		mutable std::shared_ptr<RealtimeObjectT> m_Instance;

		static result<std::shared_ptr<RealtimeObjectT>> load(const std::string& path);
	};
}

template<typename RealtimeT>
constexpr seal::resource_ref<RealtimeT>::resource_ref(std::string path)
	: m_Path(std::move(path))
	, m_Instance(nullptr)
{}

template<typename RealtimeT>
seal::result<std::shared_ptr<RealtimeT>> seal::resource_ref<RealtimeT>::load(const std::string& path)
{
	static std::unordered_map<std::string, std::weak_ptr<object_t>> s_Loaded{};

	auto weak_ref = s_Loaded.find(path);
	if(s_Loaded.end() == weak_ref || weak_ref->second.expired()) {
		auto resource = seal::load_resource(path);
		// Check if we failed to load the resource
		if(!resource) {
			seal::log::error("Failed to load resource: {}", path);
			return resource.error();
		}

		auto parsed_resource = resource_loader<object_t>::load(*resource);
		static_assert(std::is_same_v<decltype(parsed_resource), result<std::shared_ptr<object_t>>>,
					  "resource_loader<RealtimeT>::load doesnt return a seal::result<std::shared_ptr<RealtimeT>>");

		if(!parsed_resource) {
			seal::log::error("Failed to parse resource: {}", path);
			return parsed_resource.error();
		}

		s_Loaded[path] = *parsed_resource;
		return std::move(*parsed_resource);
	}

	// If we already have a second one we just lock it.
	return weak_ref->second.lock();
}
