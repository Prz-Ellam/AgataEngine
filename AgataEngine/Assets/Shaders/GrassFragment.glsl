#version 330 core

in vec2 fs_TexCoords;
in vec3 fs_UnitNormal;
in vec3 fs_ToLight;
flat in float fs_TexIndex;

out vec4 o_Color;

uniform sampler2D u_Textures[32];
uniform vec4 u_LightColour;

void main() {

	vec4 ambient = vec4(0.1f, 0.1f, 0.1f, 1.0f);

	float diffuseFactor = dot(fs_ToLight, fs_UnitNormal);
	diffuseFactor = max(diffuseFactor, 0.0f);
	vec4 diffuse = diffuseFactor * u_LightColour;

	int texIndex = int(fs_TexIndex);
	vec4 texColor = texture(u_Textures[texIndex], fs_TexCoords);

	if (texColor.a < 0.5f) {
		discard;
	}

	o_Color = texColor * (diffuse + ambient);

}