#version 330 core

out vec4 o_Color;

in vec2 fs_TexCoords;
//in vec4 fs_ClipSpace;

uniform sampler2D u_Texture;

void main() {

	//vec2 ndc = (fs_ClipSpace.xy / fs_ClipSpace.w) / 2.0f + 0.5f;
	//vec2 refractionTexCoords = vec2(ndc.x, ndc.y);

	o_Color = texture(u_Texture, fs_TexCoords);

	//float z = gl_FragCoord.z * 2.0 - 1.0;
    //float depth = (2.0 * 0.1 * 100) / (0.1 + 100 - z * (0.1 - 100));
	//
	//o_Color = vec4(vec3(depth), 1.0);

}