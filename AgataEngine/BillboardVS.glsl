#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normals;

out vec2 fs_TexCoords;
out vec3 fs_UnitNormal;
out vec3 fs_ToLight;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;

void main() {

	vec4 worldPos = u_Model * vec4(a_Pos, 1.0f);
	gl_Position = u_Projection * u_View * worldPos;
	fs_TexCoords = a_TexCoords;

	fs_UnitNormal = normalize(vec3(u_Model * vec4(a_Normals, 0.0f)));
	fs_ToLight = normalize(u_LightPos - worldPos.xyz);

}