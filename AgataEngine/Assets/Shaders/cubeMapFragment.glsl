#version 330 core

out vec4 o_Color;

in vec3 fs_UnitView;
in vec2 fs_TexCoords;
in vec3 fs_Normals;
in vec3 fs_UnitNormal;

uniform samplerCube u_CubeMap;
uniform sampler2D u_Texture;
uniform mat4 u_RotCubeMap;

void main() {

	vec3 reflectionVector = reflect(fs_UnitView, fs_UnitNormal);
	//reflectionVector = (u_RotCubeMap * vec4(reflectionVector, 1.0f)).xyz;

	vec4 reflectionColour = texture(u_CubeMap, reflectionVector);
	vec4 colorTex = texture(u_Texture, fs_TexCoords);
	o_Color = reflectionColour;

	o_Color = mix(colorTex, reflectionColour, 0.5f);

}