#pragma once


#include "seal/api/back/pipeline.hpp"

namespace seal {

	class pipeline
	{
	public:
		constexpr pipeline() :
			m_Id(0)
		{}

		constexpr pipeline(pipeline&& other) noexcept
			: m_Id(other.m_Id)
		{
			other.m_Id = INVALID_ID;
		}

		pipeline& operator=(pipeline&& other) noexcept;

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
		constexpr pipeline(id id)
			: m_Id(id)
		{}

		void release() noexcept;

		id m_Id;
	};

}
