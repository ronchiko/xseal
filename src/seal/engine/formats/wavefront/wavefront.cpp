
#include "seal/engine/formats/wavefront/wavefront.hpp"
#include "seal/engine/formats/wavefront/format.hpp"
#include "seal/resource/interpreters/string_resource.hpp"
#include "seal/types/ext/container.hpp"
#include "seal/types/ext/string.hpp"
#include "seal/types/ranges/enumerate.hpp"
#include "seal/types/ranges/tokenize.hpp"

namespace seal {
	namespace {
		class wavefront_loader
		{
		public:
			explicit wavefront_loader(resource resource)
				: m_Resource(std::move(resource))
				, m_Points()
				, m_Parsed()
			{}

			void load()
			{
				m_Parsed = {};
				m_Points = {};

				for(auto line = m_Resource.read_line(); !line.empty();
					line = m_Resource.read_line()) {
					parse_line(line);
				}
			}

		private:
			using face_type = std::tuple<u32, std::optional<u32>, std::optional<u32>>;

			/**
				Parses a line.
			 */
			void parse_line(const std::string_view line)
			{
				auto tokenizer = ranges::tokenize(line, ' ');
				dispatch_command(tokenizer);
			}

			/**
				Given a token collection return parses them into the wavefront.

				\param token: The token list.
			 */
			void dispatch_command(ranges::detail::tokenizer_view<char>& token)
			{
				const auto semantic = *token;
				++token;

				switch(seal::convert<wavefront::wavefront_format_entry>(semantic)) {
				case wavefront::wavefront_format_entry::Vertex:
					m_Parsed.vertices.push_back(parse_vector<4>(token, 3, 1.0));
					break;
				case wavefront::wavefront_format_entry::Uv:
					m_Parsed.uvs.push_back(parse_vector<3>(token, 2, 0.0));
					break;
				case wavefront::wavefront_format_entry::Normal:
					m_Parsed.normals.push_back(parse_vector<3>(token));
					break;
				case wavefront::wavefront_format_entry::Face:
					handle_face(token);
					break;
				// Handle comments
				case wavefront::wavefront_format_entry::Comment:
				default:
					if(!semantic.empty() && semantic.at(0) == '#') {
						break;
					}

					throw seal::failure("Unknown wavefront semantic: {}", semantic);
				}
			}

			/**
				Attempt to handle a face OBJ format.

				\param token:  The token to view.
			 */
			void handle_face(const ranges::detail::tokenizer_view<char>& token)
			{
				for(const auto point : token) {
					auto [vertex, uv, normal] = parse_face_point(point);

					wavefront::wavefront_point wavefront_point{
						m_Parsed.vertices[vertex],
						uv.has_value() ? m_Parsed.uvs[uv.value()] : v3<f32>{ 0, 0, 0 },
						normal.has_value() ? m_Parsed.normals[normal.value()] : v3<f32>{ 0, 0, 0 },
					};

					m_Points.push_back(wavefront_point);
				}
			}

			/**
				Attempts to parse a face point from a token. Returns a tuple with the found indices.

				\param token: The token to parse.


			 */
			[[nodiscard]] face_type parse_face_point(const std::string_view token) const
			{
				std::optional<i32> indices[3] = { {}, {}, {} };

				for(const auto [index, value] : ranges::tokenize(token, '/') | ranges::enumerate()) {
					if(index >= 3) {
						log::error("Face is too large: {}", token);
						throw failure("Too many parts to face.");
					}

					indices[index] = parse_face_index(value, index);
				}

				if(!indices[0].has_value()) {
					log::error("Invalid face format: {}", token);
					throw failure("Invalid face format.");
				}

				return std::make_tuple(indices[0].value(), indices[1], indices[2]);
			}

			/**
				Given a set of tokens parses them into vectors.

				\tparam Size: The desired size of the vector.
				\param token: The tokens to parse
				\param min_allowed: The minimum allowed elements in the vector.
				\param default_value: The value to set to undefined vector elements.
			 */
			template<size_t Size>
			glm::vec<Size, f32> parse_vector(ranges::detail::tokenizer_view<char>& token,
											 size_t min_allowed = Size,
											 f32 default_value = 0.0)
			{
				// First parse the required part and keep track of how much we parsed
				size_t i = 0;
				glm::vec<Size, f32> result;
				for(; token != ranges::detail::tokenizer_view<char>::end() && i < Size;
					++i, ++token) {
					result[static_cast<i32>(i)] = string::to_f32(*token);
				}

				// Check we have parsed enough
				if(min_allowed > i) {
					throw failure("Too few arguments for wavefront semantic. expected {}, got {}",
								  min_allowed,
								  i);
				}

				// Fill the rest of the vector.
				for(; i < Size; ++i) {
					result[static_cast<i32>(i)] = default_value;
				}

				return result;
			}

			/**
				Attempts to parse a face index, according to the wavefront standard:
				* The first index in an array is 1
				* If the face_index is negative, lookup the index in reverse
				* Zero is not allowed.

				\param face_index: The face index to parse.
				\param index: The index of the face index to parse (0 - vertex, 1 - uv, 2- normal)
			 */
			[[nodiscard]] u32 parse_face_index(const std::string_view face_index,
											   const u32 index) const
			{
				const i32 sizes[3] = { static_cast<i32>(m_Parsed.vertices.size()),
									   static_cast<i32>(m_Parsed.uvs.size()),
									   static_cast<i32>(m_Parsed.normals.size()) };

				auto vertex_index = string::to_int<i32>(face_index);
				if(0 == vertex_index) {
					log::info("Invalid OBJ model, face index is zero");
					throw failure("Invalid OBJ model, got zero index");
				}

				if(vertex_index > 0) {
					--vertex_index;
				} else if(vertex_index < 0) {
					vertex_index += sizes[index];
				}

				return vertex_index;
			}

			struct parsed_info
			{
				std::vector<v4<f32>> vertices;
				std::vector<v3<f32>> uvs;
				std::vector<v3<f32>> normals;
			};

			string_resource m_Resource;
			std::vector<wavefront::wavefront_point> m_Points;
			parsed_info m_Parsed;
		};
	}

	wavefront_object load_wavefront(const resource& resource)
	{
		wavefront_loader loader(resource);

		loader.load();
		return wavefront_object{};
	}

}
