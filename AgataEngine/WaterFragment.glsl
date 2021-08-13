#version 330 core

layout (location = 0) out vec4 o_FragColor;

in vec2 fs_TexCoords;
in vec4 fs_ClipSpace;
in vec3 fs_ToCameraVector;
in vec3 fs_LightDirection;

uniform sampler2D u_Refraction;
uniform sampler2D u_Reflection;
uniform sampler2D u_dudv;
uniform sampler2D u_NormalMap;
uniform sampler2D u_DepthMap;
uniform float u_MoveFactor;
uniform vec3 u_LightColour;

void main() {

	// Normalize device coordinates
	vec2 ndc = (fs_ClipSpace.xy / fs_ClipSpace.w) / 2.0f + 0.5f;
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);
	vec2 reflectionTexCoords = vec2(ndc.x, 1 - ndc.y);

	// Distortion
	vec4 dudvMap = texture(u_dudv, vec2(fs_TexCoords.x + u_MoveFactor, fs_TexCoords.y));
	vec2 distortedTexCoords = dudvMap.rg * 0.2f; // tiling factor
	distortedTexCoords = fs_TexCoords + vec2(distortedTexCoords.x, distortedTexCoords.y + u_MoveFactor);
	dudvMap = texture(u_dudv, distortedTexCoords);
	vec2 distortion = (dudvMap.rg * 2.0f - 1.0f) * 0.02f; // Distortion strength

	refractionTexCoords += distortion;
	refractionTexCoords = clamp(refractionTexCoords, 0.001f, 0.999f);
	reflectionTexCoords += distortion;
	reflectionTexCoords = clamp(reflectionTexCoords, 0.001f, 0.999f);

	vec4 refraction = texture(u_Refraction, refractionTexCoords);
	vec4 reflection = texture(u_Reflection, reflectionTexCoords);

	// Normals
	vec4 normalMap = texture(u_NormalMap, distortedTexCoords);
	vec3 normal = normalize(vec3(normalMap.r * 2.0f - 1.0f, normalMap.b, normalMap.g * 2.0f - 1.0f));

	float reflectivity = 1.0f;
	vec3 reflectLight = reflect(fs_LightDirection, normal);
	float specularScalar = dot(reflectLight, fs_ToCameraVector);
	specularScalar = max(specularScalar, 0.0f);
	specularScalar = pow(specularScalar, 20);
	vec3 specularLight = u_LightColour * specularScalar * reflectivity;

	// Fresnel effect
	float reflectivityFactor = dot(fs_ToCameraVector, vec3(0.0f, 1.0f, 0.0f));
	reflectivityFactor = pow(reflectivityFactor, 0.3f);

	// Final color output
	o_FragColor = mix(reflection, refraction, reflectivityFactor);
	o_FragColor = mix(o_FragColor, vec4(0.0f, 0.3f, 0.5f, 1.0f), 0.2f) + vec4(specularLight, 0.0f);

	//float depth = texture(u_DepthMap, refractionTexCoords).r;
	//float distance = 2.0f * 0.1f * 100.0f / (100.0f + 0.1f - (2.0f  * depth - 1.0f) * (100.0f - 0.1f));
	//depth = gl_FragCoord.z;
	//float waterDistance = 2.0f * 0.1f * 100.0f / (100.0f + 0.1f - (2.0f  * depth - 1.0f) * (100.0f - 0.1f));
	//float waterDepth = distance - waterDistance;
	//
	//o_Color = vec4(vec3(waterDepth / 50.0f), 1.0f);
}