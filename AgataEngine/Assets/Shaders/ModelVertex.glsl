#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Normals;
layout(location = 3) in vec3 a_Tangents;
layout(location = 4) in vec3 a_Bitangents;

out vec2 fs_TexCoords;
out vec3 fs_ToLightVector;
out vec3 fs_ToCameraVector;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform vec4 u_Plane;

void main() {

	vec4 worldPos = u_Model * vec4(a_Pos, 1.0f);
	gl_ClipDistance[0] = dot(worldPos, u_Plane);
	gl_Position = u_Projection * u_View * worldPos;

	fs_TexCoords = a_TexCoords;

	vec3 T = normalize(vec3(u_Model * vec4(a_Tangents, 0.0f)));
	vec3 B = normalize(vec3(u_Model * vec4(a_Bitangents, 0.0f)));
	vec3 N = normalize(vec3(u_Model * vec4(a_Normals, 0.0f)));
	mat3 TBN = transpose(mat3(T, B, N));

	fs_ToLightVector = normalize(TBN * (u_LightPos - worldPos.xyz));
	fs_ToCameraVector = normalize(TBN * (u_CameraPos - worldPos.xyz));

}