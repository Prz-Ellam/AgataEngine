#version 330 core

layout (location = 0) out vec4 o_FragColor;

in vec2 fs_TexCoords;

uniform sampler2D u_Shape;

void main() {

	vec4 shape = texture(u_Shape, fs_TexCoords);
	shape.a = 1.2f - ((shape.r + shape.g + shape.b) / 3.0f);

	o_FragColor = shape;

}