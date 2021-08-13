#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normals;

out vec3 fs_UnitView;
out vec2 fs_TexCoords;
out vec3 fs_Normals;
out vec3 fs_UnitNormal;

uniform vec3 u_CameraPos;
uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {

	vec4 worldPos = u_Model * vec4(a_Pos, 1.0f);
	gl_Position = u_Projection * u_View * worldPos;

	fs_UnitView = normalize(worldPos.xyz - u_CameraPos);

	fs_UnitNormal = normalize(a_Normals);
	fs_Normals = a_Normals;
	fs_TexCoords = a_TexCoords;

}