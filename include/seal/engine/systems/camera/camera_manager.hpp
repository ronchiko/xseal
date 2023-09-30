#pragma once

#include "seal/types.hpp"
#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/systems/camera/camera.hpp"
#include "seal/engine/render/pipeline/pipeline_data_provider.hpp"

namespace seal::system
{
	/**
		The name of the pipeline provider the camera manager system uses to pass down information
		to pipelines.

		If you are writing a custom renderer and is interested in getting camera information in your
		shader use this name to acquire a provider. 
	 */
	inline constexpr std::string_view CAMERA_PIPELINE_PROVIDER_NAME = "seal-global-matrices";

	/**
		The information the camera manager passes down the pipeline each frame.

		Contains the projection matrix for this and the view matrix of the active camera.
	 */
	struct camera_shader_uniform
	{
		m4<f32> projection_matrix;
		m4<f32> view_matrix;
	};

	class camera_manager
	{
	public:
		void initialize();

		void update(const entity& e, const transform& transform, const camera& camera);

		void update_last();

	private:
		static constexpr u32 NO_ACTIVE_CAMERA = std::numeric_limits<u32>::max();

		using provider_t = pipeline_data_provider<camera_shader_uniform>;

		bool m_PushedMatrices = false;
		provider_t m_PipelineProvider; 
	};	
}
