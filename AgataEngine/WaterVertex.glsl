#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;

out vec2 fs_TexCoords;
out vec4 fs_ClipSpace;
out vec3 fs_ToCameraVector;
out vec3 fs_LightDirection;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPosition;
uniform vec3 u_LightPosition;

void main() {

	vec4 worldPos = u_Model * vec4(a_Pos, 1.0f);
	fs_ClipSpace = u_Projection * u_View * worldPos;
	gl_Position = fs_ClipSpace;

	fs_TexCoords = a_TexCoords;

	fs_ToCameraVector = normalize(u_CameraPosition - worldPos.xyz);
	fs_LightDirection = normalize(worldPos.xyz - u_LightPosition);

}