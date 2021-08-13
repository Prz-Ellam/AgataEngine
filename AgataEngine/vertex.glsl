#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;

out vec2 fs_TexCoords;
out vec4 fs_ClipSpace;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {

	fs_ClipSpace = vec4(a_Pos, 1.0f);
	gl_Position = fs_ClipSpace;
	fs_TexCoords = a_TexCoords;

}