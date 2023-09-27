#version 300 es

// Provided by the world camera
layout(std140) uniform Seal_GlobalMatrices
{
	mat4x4 ProjectMatrix;
	mat4x4 ViewMatrix;
};

in vec3 Seal_Vertex;
in vec2 Seal_UV;
in vec4 Seal_Tint;

out vec4 VertexColor;
out vec2 UvCoordinate;

void main()
{
	gl_Position = vec4(Seal_Vertex, 1.0);

	VertexColor = Seal_Tint;
	UvCoordinate = Seal_UV;
}