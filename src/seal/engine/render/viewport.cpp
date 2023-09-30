#include "seal/engine/render/viewport.hpp"

#include "seal/api/back/backend.hpp"
#include "seal/api/core/display.hpp"

namespace seal {
	namespace {
		struct viewport_cache
		{
			v2<f32> view;
		} g_ViewportCache;
	}

	void api::signal_viewport_change(const v2<f32> new_viewport_size)
	{
		g_ViewportCache.view = new_viewport_size;
		update_resolution(new_viewport_size);
	}

	namespace viewport {

		v2<f32> size()
		{
			return g_ViewportCache.view;
		}

		f32 ratio()
		{
			const auto size = ::seal::viewport::size();
			return size.x / size.y;
		}
	}

}
