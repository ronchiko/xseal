#pragma once

#include <span>
#include <string>
#include <memory>

#include "seal/types.hpp"

namespace seal {
	class resource_interface
	{
	public:
		virtual ~resource_interface() = default;

		/**
		   Loads the whole resource into a vector.
		  
		   \return: The loaded resource as a readonly span.
		 */
		virtual std::span<const u8> load_all() = 0;

		/**
		   Loads an a part of the resource into a span.
		  
		   \param bytes_to_read: The amount of bytes to read.
		 */
		virtual std::span<const u8> load(u32 bytes_to_read) = 0;
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

		[[nodiscard]] std::span<const u8> load_all() const;
		[[nodiscard]] std::span<const u8> load(u32 bytes) const;

		/**
		   The Id of the resource.
		 */
		[[nodiscard]] constexpr id id() const {
			return m_Id;
		}

		/**
		   The path to the resource.
		 */
		[[nodiscard]] constexpr const std::string& path() const {
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
					 ::\<resource-path>	- Loads an embedded resource
					 <package-name>::\<resource-path> - Loads a resource from an asset package
					 anything else - Load resource from path
	 */
	resource load_resource(const std::string& path);

	/**
		Registers an embedded resource into.
	 
		\param path: The location of the resource
		\param data: The contents of the resource
		\param size: The size in bytes of data
	 */
	void add_embedded_resource(std::string path, const void *data, size_t size);
}

#define EMBEDDED_RESOURCE(name) "::\\"##name
