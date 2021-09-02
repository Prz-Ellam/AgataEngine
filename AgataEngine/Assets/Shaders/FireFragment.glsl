#version 330 core

layout (location = 0) out vec4 o_FragColor;

in vec2 fs_TexCoords;
uniform sampler2D u_Noise;
uniform sampler2D u_Alpha;
uniform sampler2D u_Ramp;
uniform float u_Displacement;
uniform sampler2D u_dudv;

void main() {

	vec2 dudv = texture(u_dudv, fs_TexCoords).rg * 0.05f;
	vec4 noiseTex = texture(u_Noise, vec2(fs_TexCoords.x + dudv.x, fs_TexCoords.y + dudv.y - u_Displacement)) * 2.0f - 1.0f;
	vec4 alphaTex = texture(u_Alpha, vec2(fs_TexCoords.x + dudv.x, fs_TexCoords.y)); 
	vec4 rampTex = texture(u_Ramp, vec2(fs_TexCoords.x + dudv.x, fs_TexCoords.y + dudv.y));

	o_FragColor = mix(noiseTex, rampTex, 0.8f);
	o_FragColor.r += 0.1f;
	o_FragColor.a = (alphaTex.r + alphaTex.g + alphaTex.b) / 3.0f;

}