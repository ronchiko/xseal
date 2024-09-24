#pragma once


#include "seal/types/conversion_table.hpp"

namespace seal::wavefront {
	enum class wavefront_format_entry
	{
		Comment,

		Vertex,
		Uv,
		Normal,
		Face,
	};

	struct wavefront_point
	{
		v4<f32> location;
		v3<f32> uv;
		v3<f32> normal;
	};
}

SEAL_BI_CONVERSION_TABLE(seal::wavefront::wavefront_format_entry,
						 std::string_view,
						 {
							 { wavefront::wavefront_format_entry::Vertex, "v" },
							 { wavefront::wavefront_format_entry::Uv, "vt" },
							 { wavefront::wavefront_format_entry::Normal, "vn" },
							 { wavefront::wavefront_format_entry::Face, "f" },
							 { wavefront::wavefront_format_entry::Comment, "#" },
						 });
