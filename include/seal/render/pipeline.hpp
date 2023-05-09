#pragma once

#include "seal/api/tagged_object.hpp"
#include "seal/api/back/pipeline.hpp"

namespace seal {

	class pipeline
	{
	public:
		constexpr pipeline() :
			m_Id(api::UNTAGGED_OBJECT)
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

	private:
		constexpr pipeline(api::abstract_t id)
			: m_Id(std::move(id))
		{}

		void release() noexcept;

		api::abstract_t m_Id;
	};

}
