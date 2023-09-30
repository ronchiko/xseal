#include "seal/engine/systems/camera/camera_manager.hpp"

#include "seal/engine/systems/camera/camera.hpp"

namespace seal::system {
	void camera_manager::initialize()
	{
		m_PipelineProvider = provider_t::create(CAMERA_PIPELINE_PROVIDER_NAME);
	}

	void camera_manager::update(const entity& e, const transform& transform, const camera& camera)
	{
		// Camera is not active, skip it.
		if(!camera.is_active()) {
			return;
		}

		// We already uploaded a a matrix to the pipelines, print an error.
		if(m_PushedMatrices) {
			log::error("More then one active camera!");
			return;
		}

		// Send the info to the shader.
		const m4<f32> rotation_matrix = mat4_cast(transform.rotation);

		auto view_matrix = translate(glm::identity<m4<f32>>(), -transform.position);
		view_matrix *= rotation_matrix;

		m_PushedMatrices = true;
		m_PipelineProvider.update(camera_shader_uniform{
			camera.compute_matrices(transform),
			view_matrix,
		});
	}

	void camera_manager::update_last()
	{
		m_PushedMatrices = false;
	}

}
