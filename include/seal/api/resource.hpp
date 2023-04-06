#pragma once

#include <vector>
#include <span>

#include "seal/types.hpp"
#include "seal/types/result.hpp"

namespace seal {
	class resource_interface
	{
	public:
		virtual ~resource_interface() = default;

		/**
		   Loads the whole resource into a vector.
		  
		   \return: The loaded resource as a readonly span.
		 */
		virtual result<std::span<const u8>> load_all() = 0;

		/**
		   Loads an a part of the resource into a span.
		  
		   \param span: The span to put the result into
		   \returns: The amount of written to the input span
		 */
		virtual result<u32> load(std::span<u8> span) = 0;
	};

	/**
	   A resource with its identification data.
	 */
	class unique_resource
	{
	public:
		explicit unique_resource(std::string path, resource_interface *resource);

		constexpr unique_resource(unique_resource&& other) noexcept 
			: m_Path(std::move(other.m_Path))
			, m_Id(other.m_Id)
			, m_Resource(other.m_Resource)
		{
			other.reset();
		}

		unique_resource& operator=(unique_resource&& other) noexcept;

		unique_resource(const unique_resource&) = delete;
		unique_resource& operator=(const unique_resource&) = delete;

		~unique_resource() noexcept;

		result<std::span<const u8>> load_all();
		result<u32> load(std::span<u8> span);

		/**
		   The Id of the resource.
		 */
		constexpr id id() const {
			return m_Id;
		}

		/**
		   The path to the resource.
		 */
		constexpr const std::string& path() const {
			return m_Path;
		}

	private:
		constexpr void reset() noexcept {
			m_Path = "";
			m_Id = INVALID_ID;
			m_Resource = nullptr;
		}

		void release() noexcept;

		std::string m_Path;
		seal::id m_Id;
		resource_interface *m_Resource;
	};

	using resource = std::shared_ptr<unique_resource>;

	/**
		Loads a resource.

		\param path: The path to the resource.
					 ::\<resource-path>	- Loads an embeded resource
					 <package-name>::\<resource-path> - Loads a resource from an asset package
					 anything else - Load resource from path
	 */
	result<resource> load_resource(const std::string& path);

	/**
		Registers an embedded resource into.
	 
		\param data: The contents of the resource
		\param size: The size in bytes of data
	 */
	result<void> add_embedded_resource(std::string path, const void *data, size_t size);
}

#define EMBEDDED_RESOURCE(name) "::\\"##name
