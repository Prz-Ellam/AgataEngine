#version 330 core

layout (location = 0) out vec4 o_FragColor;

in vec3 fs_TexCoords;

uniform samplerCube u_CubeMap;

void main() {

	o_FragColor = texture(u_CubeMap, fs_TexCoords);

}