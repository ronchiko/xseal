precision mediump float;

uniform sampler2D Texture;

varying vec4 VertexColor;
varying vec2 UvCoordinate;

void main()
{
	gl_FragColor = VertexColor * texture2D(Texture, UvCoordinate);
}