#version 100

attribute vec3 Seal_Vertex;
attribute vec2 Seal_UV;
attribute vec4 Seal_Tint;

varying vec4 vertex_color;

void main()
{
	gl_Position = vec4(Seal_Vertex, 1.0);

	vertex_color = Seal_Tint;
}