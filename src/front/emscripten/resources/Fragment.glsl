#version 300 es

precision mediump float;

uniform sampler2D Texture;

in vec4 VertexColor;
in vec2 UvCoordinate;

out vec4 Seal_FragmentColor;

void main()
{
	Seal_FragmentColor = VertexColor * texture(Texture, UvCoordinate);
}