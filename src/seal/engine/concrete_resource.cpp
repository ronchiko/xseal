#include "seal/engine/resource.hpp"


namespace seal
{
	void resource_collection::attach(std::unique_ptr<resource_collection_object> resource)
	{
		m_Resources.push_back(std::move(resource));
	}

}
