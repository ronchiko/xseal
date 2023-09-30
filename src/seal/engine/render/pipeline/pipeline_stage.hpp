#pragma once

#include "seal/api/back/pipeline.hpp"
#include "seal/api/tagged_object.hpp"
#include "seal/engine/resource.hpp"
#include "seal/engine/render/texture2d.hpp"

#include "seal/opt/json.hpp"

namespace seal {
	namespace detail {
		template<api::pipeline_stage_type Ty>
		struct pipeline_type_constructor;

		template<typename Ty>
		struct pipeline_constructor_enum;

#define SEAL_DEFINE_PIPELINE_CONSTRUCTOR(enum_type, constructor_type)                              \
	template<>                                                                                     \
	struct pipeline_type_constructor<api::pipeline_stage_type::enum_type>                          \
	{                                                                                              \
		using type = api::pipeline_stage_creation_options::constructor_type;                       \
	};                                                                                             \
	template<>                                                                                     \
	struct pipeline_constructor_enum<api::pipeline_stage_creation_options::constructor_type>       \
	{                                                                                              \
		static constexpr auto value = api::pipeline_stage_type::enum_type;                         \
	}

		SEAL_DEFINE_PIPELINE_CONSTRUCTOR(GraphicsPipeline, graphics);
		SEAL_DEFINE_PIPELINE_CONSTRUCTOR(ComputationPipeline, compute);


#undef SEAL_DEFINE_PIPELINE_CONSTRUCTOR
	}

	class pipeline_stage
	{
	public:
		using uniforms_vector = std::vector<api::uniform_declaration_options>;

		explicit pipeline_stage()
			: m_Id(api::UNTAGGED_OBJECT)
		{}

		constexpr pipeline_stage(pipeline_stage&& other) noexcept = default;

		pipeline_stage& operator=(pipeline_stage&& other) noexcept = default;

		pipeline_stage(const pipeline_stage&) = delete;
		pipeline_stage& operator=(const pipeline_stage&) = delete;

		~pipeline_stage() noexcept;

		/**
		   Creates a graphics pipeline.

		   \param settings: The settings the pipeline will use.
		   \param uniforms: The uniforms in this stage.
		 */
		template<typename ConstructorType>
		static pipeline_stage create(ConstructorType settings, uniforms_vector uniforms);

		/**
		   Binds the pipeline to be the active one.
		 */
		void bind() const;

		/**
			Query this stage for a uniform.

			\param name: The name of the uniform.
		 */
		[[nodiscard]]
		api::uniform_query_information query_uniform(const std::string& name) const;

		/**
			Updates a uniforms value.
		 */
		void update_uniform(u32 location, u32 value) const;

		bool operator==(const pipeline_stage& other) const;
		bool operator!=(const pipeline_stage& other) const;

		constexpr operator api::abstract_t() const
		{
			return m_Id;
		}

	private:
		explicit pipeline_stage(api::abstract_t id)
			: m_Id(std::move(id))
		{}

		void release() noexcept;

		api::abstract_t m_Id;
	};
}

template<typename ConstructorType>
seal::pipeline_stage seal::pipeline_stage::create(ConstructorType settings, uniforms_vector uniforms)
{
	const api::pipeline_stage_creation_options options{
		detail::pipeline_constructor_enum<std::decay_t<ConstructorType>>::value,
		settings,
		std::move(uniforms),
	};

	return pipeline_stage(api::create_pipeline_stage(options));
}
