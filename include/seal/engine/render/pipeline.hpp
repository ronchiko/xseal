#pragma once

#include "seal/api/back/pipeline.hpp"
#include "seal/api/tagged_object.hpp"

#include "seal/engine/resource.hpp"
#include "seal/opt/json.hpp"

namespace seal {

	class pipeline
	{
	public:
		constexpr pipeline()
			: m_Id(api::UNTAGGED_OBJECT)
		{}

		constexpr pipeline(pipeline&& other) noexcept = default;

		pipeline& operator=(pipeline&& other) noexcept = default;

		pipeline(const pipeline&) = delete;
		pipeline& operator=(const pipeline&) = delete;

		~pipeline() noexcept;

		/**
		   Creates a graphics pipeline.

		   \param settings: The settings the pipeline will use.
		 */
		static result<pipeline> create_graphics(api::pipeline_description::graphics settings);

		/**
		   Binds the pipeline to be the active one.
		 */
		void bind() const;

		bool operator==(const pipeline& other) const;
		bool operator!=(const pipeline& other) const;

	private:
		constexpr pipeline(api::abstract_t id)
			: m_Id(std::move(id))
		{}

		void release() noexcept;

		api::abstract_t m_Id;
	};

}


/*
 *	Resource loader for a pipeline.
 */ 
template<>
class seal::resource_loader<seal::pipeline>
{
public:
	static seal::result<std::shared_ptr<seal::pipeline>> load(seal::resource& resource);

private:
	static api::pipeline_description::type_t parse_type_string(const std::string& data);
};