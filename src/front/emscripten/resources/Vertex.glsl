#version 100

attribute vec3 position;
attribute vec2 uv;
attribute vec4 color;

varying vec4 vertex_color;

void main()
{
	gl_Position = vec4(position, 1.0);

	vertex_color = color;
}