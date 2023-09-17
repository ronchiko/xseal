#version 100

attribute vec3 Seal_Vertex;
attribute vec2 Seal_UV;
attribute vec4 Seal_Tint;

varying vec4 VertexColor;
varying vec2 UvCoordinate;

void main()
{
	gl_Position = vec4(Seal_Vertex, 1.0);

	VertexColor = Seal_Tint;
	UvCoordinate = Seal_UV;
}