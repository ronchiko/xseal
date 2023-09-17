#version 450 core

precision mediump float;

layout(binding = 0) uniform sampler2D Texture;

in vec2 UvCoordinate;
in vec4 VertexColor;

out vec4 Seal_FragmentColor;

void main()
{
	Seal_FragmentColor = VertexColor * texture2D(Texture, UvCoordinate);
}