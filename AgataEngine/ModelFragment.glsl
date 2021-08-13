#version 330 core

layout (location = 0) out vec4 o_FragColor;

in vec2 fs_TexCoords;
in vec3 fs_ToLightVector;
in vec3 fs_ToCameraVector;

uniform vec3 u_LightColour;

uniform vec3 u_AmbientMaterial;
uniform vec3 u_DiffuseMaterial;
uniform vec3 u_SpecularMaterial;
uniform float u_ShininessMaterial;
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform sampler2D u_NormalMap;

void main() {

	// Normal Map
	vec3 unitNormal = 2.0f * texture(u_NormalMap, fs_TexCoords).rgb - 1.0f;
	unitNormal = normalize(unitNormal);

	// Ambient Light
	vec3 ambient = u_AmbientMaterial * u_LightColour;

	// Diffuse Light
	float diffuseScalar = dot(unitNormal, fs_ToLightVector);
	diffuseScalar = max(diffuseScalar, 0.0f);
	vec3 diffuse = diffuseScalar * u_DiffuseMaterial * u_LightColour;
	vec4 diffuseColor = texture(u_DiffuseMap, fs_TexCoords);

	// Specular Light
	vec3 lightDirection = -fs_ToLightVector;
	vec3 reflectLightDir = reflect(lightDirection, unitNormal);
	float specularScalar = dot(reflectLightDir, fs_ToCameraVector);
	specularScalar = max(specularScalar, 0.0f);
	specularScalar = pow(specularScalar, u_ShininessMaterial);
	vec3 specular = specularScalar * u_SpecularMaterial * u_LightColour;
	vec4 specularMap = texture(u_SpecularMap, fs_TexCoords);
	specular *= (specularMap.r + specularMap.g + specularMap.b) / 3.0f;

	// Final Output
	o_FragColor = diffuseColor * vec4(diffuse + ambient + specular, 1.0f);

}