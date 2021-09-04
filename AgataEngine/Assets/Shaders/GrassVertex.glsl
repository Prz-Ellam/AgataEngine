#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Normals;
layout (location = 3) in vec3 a_Tangents;
layout (location = 4) in vec3 a_Bitangents;
layout (location = 5) in float a_TexIndex;
layout (location = 6) in float a_Displacement;

out vec2 fs_TexCoords;
out vec3 fs_UnitNormal;
out vec3 fs_ToLight;
out int fs_TexIndex;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform float u_Time;

void main() {

	vec3 skewPos = a_Pos;
	if (a_TexCoords.y == 1.0f) {
		skewPos.x += sin(u_Time + a_Displacement) * 0.1f;
	}

	vec4 worldPos = vec4(skewPos, 1.0f);
	gl_Position = u_Projection * u_View * worldPos;

	fs_TexCoords = a_TexCoords;
	fs_TexIndex = int(a_TexIndex);

	fs_UnitNormal = normalize(vec3(vec4(a_Normals, 0.0f)));
	fs_ToLight = normalize(u_LightPos - worldPos.xyz);

}