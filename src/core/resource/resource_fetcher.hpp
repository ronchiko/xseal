#pragma once

#include "seal/api/resource.hpp"

namespace seal {
	/**
	   Creates a resource object.
	 */
	template<typename T, typename... Args>
	resource_interface *make_resource(Args&&...args)
	{
		return new T(args...);
	}

	class resource_fetcher
	{
	public:
		virtual ~resource_fetcher() = default;

		/**
		   Fetches a resource using its path.
		  
		   \param path: The path to fetch
		   \note: The allocated resource is owned by the caller
		 */
		virtual resource_interface* fetch(const std::string& path) = 0;
	};
}
