#version 450 core

// Provided by the world camera
layout(std140, binding = 0) uniform Seal_GlobalMatrices
{
	mat4x4 ProjectMatrix;
	mat4x4 ViewMatrix;
};

layout(location = 0) in vec3 Seal_Vertex;
layout(location = 1) in vec2 Seal_UV;
layout(location = 2) in vec4 Seal_Tint;

out vec4 VertexColor;
out vec2 UvCoordinate;

void main()
{
	gl_Position = vec4(Seal_Vertex, 1.0);

	VertexColor = Seal_Tint;
	UvCoordinate = Seal_UV;
}