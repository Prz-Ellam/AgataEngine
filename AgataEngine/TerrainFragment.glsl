#version 330 core

layout (location = 0) out vec4 o_FragColor;

in vec2 fs_TexCoords;
in vec3 fs_ToLightVector;
in vec3 fs_ToCameraVector;

uniform vec3 u_LightColour;

uniform sampler2D u_RedDiffuse;
uniform sampler2D u_RedNormal;
// uniform sampler2D u_RedSpecular;
uniform sampler2D u_GreenDiffuse;
uniform sampler2D u_GreenNormal;
// uniform sampler2D u_GreenSpecular;
uniform sampler2D u_BlueDiffuse;
uniform sampler2D u_BlueNormal;
// uniform sampler2D u_BlueSpecular;
uniform sampler2D u_Diffuse;
uniform sampler2D u_Normal;
// uniform sampler2D u_Specular;
uniform sampler2D u_BlendMap;

uniform float u_ShineDamper;
uniform float u_Reflectivity;

uniform vec3 u_AmbientMaterial;
uniform vec3 u_DiffuseMaterial;

uniform float u_TilingFactor;

vec4 blendTexture(vec4 blendMap, sampler2D background, sampler2D red, sampler2D green, sampler2D blue, vec2 texCoords);

void main() {

	// Blend Map
	vec4 blendMapColor = texture(u_BlendMap, fs_TexCoords);
	vec2 tileTexCoord = fs_TexCoords * u_TilingFactor;
	float background = 1 - (blendMapColor.r + blendMapColor.g + blendMapColor.b);

	// Normal Map
	vec4 totalNormal = blendTexture(blendMapColor, u_Normal, u_RedNormal, u_GreenNormal, u_BlueNormal, tileTexCoord);
	vec3 texNormal = totalNormal.rgb;
	texNormal = normalize(texNormal * 2.0f - 1.0f);
	
	// Diffuse Color
	vec4 diffuseColor = blendTexture(blendMapColor, u_Diffuse, u_RedDiffuse, u_GreenDiffuse, u_BlueDiffuse, tileTexCoord);

	// Ambient
	vec3 ambient = u_AmbientMaterial * u_LightColour;

	// Diffuse
	float diffuseScalar = dot(texNormal, fs_ToLightVector);
	diffuseScalar = max(diffuseScalar, 0.0f);
	vec3 diffuse = diffuseScalar * u_DiffuseMaterial * u_LightColour;

	// Specular
	vec3 lightDirection = -fs_ToLightVector;
	vec3 reflectLightDir = reflect(lightDirection, texNormal);
	float specularScalar = dot(reflectLightDir, fs_ToCameraVector);
	specularScalar = max(specularScalar, 0.0f);
	specularScalar = pow(specularScalar, u_ShineDamper);
	vec3 specular = specularScalar * u_Reflectivity * u_LightColour;

	o_FragColor = diffuseColor * vec4(ambient + diffuse, 1.0f);

}

vec4 blendTexture(vec4 blendMap, sampler2D background, sampler2D red, sampler2D green, sampler2D blue, vec2 texCoords) {

	float black = 1 - (blendMap.r + blendMap.g + blendMap.b);

	vec4 backgroundChannel = texture(background, texCoords) * black;
	vec4 redChannel = texture(red, texCoords) * blendMap.r;
	vec4 greenChannel = texture(green, texCoords) * blendMap.g;
	vec4 blueChannel = texture(blue, texCoords) * blendMap.b;

	return backgroundChannel + redChannel + greenChannel + blueChannel;

}