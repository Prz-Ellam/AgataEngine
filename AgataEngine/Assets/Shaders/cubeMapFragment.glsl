#version 330 core

out vec4 o_Color;

in vec3 fs_UnitView;
in vec2 fs_TexCoords;
in vec3 fs_Normals;
in vec3 fs_UnitNormal;

uniform samplerCube u_CubeMap;
uniform mat4 u_RotCubeMap;

void main() {

	vec3 reflectionVector = refract(fs_UnitView, fs_UnitNormal, 1.0f/1.33f);
	//reflectionVector = (u_RotCubeMap * vec4(reflectionVector, 1.0f)).xyz;

	vec4 reflectionColour = texture(u_CubeMap, reflectionVector);

	o_Color = reflectionColour;

	//o_Color = mix(vec4(0.98, 0.82, 0.76, 1.0f), reflectionColour, 0.6f);

}