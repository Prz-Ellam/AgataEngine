#version 330 core

layout (location = 0) out vec4 o_FragColor;

in vec2 fs_TexCoords;
in vec3 fs_ToLightVector;
in vec3 fs_ToCameraVector;

in vec3 fs_JointID;

uniform vec3 u_LightColour;

uniform vec3 u_AmbientMaterial;
uniform vec3 u_DiffuseMaterial;
uniform vec3 u_SpecularMaterial;
uniform float u_ShininessMaterial;
uniform sampler2D u_DiffuseMap;
uniform sampler2D u_SpecularMap;
uniform sampler2D u_NormalMap;

void main() {

	#if 0
    // Torso        
	if (fs_JointID.x == 0.0f){
		o_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
		return;
	}
	// Chest
	if (fs_JointID.x == 1.0f){
		o_FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
		return;
	}
	// Neck
	if (fs_JointID.x == 2.0f){
		o_FragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
		return;
	}
	//Head
	if (fs_JointID.x == 3.0f){
		o_FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
		return;
	}
	// Upper_Arm_L
	if (fs_JointID.x == 4.0f){
		o_FragColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
		return;
	}
	// Lower_Arm_L
	if (fs_JointID.x == 5.0f){
		o_FragColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
		return;
	}
	// Hand_L
	if (fs_JointID.x == 6.0f){
		o_FragColor = vec4(1.0f, 0.5f, 0.0f, 1.0f);
		return;
	}
	// Upper_Arm_R
	if (fs_JointID.x == 7.0f){
		o_FragColor = vec4(1.0f, 0.0f, 0.5f, 1.0f);
		return;
	}
	// Lower_Arm_R
	if (fs_JointID.x == 8.0f){
		o_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		return;
	}
	// Hand_R
	if (fs_JointID.x == 9.0f){
		o_FragColor = vec4(0.0f, 0.5f, 1.0f, 1.0f);
		return;
	}
	// Upper_Leg_L
	if (fs_JointID.x == 10.0f){
		o_FragColor = vec4(0.6f, 0.2f, 0.4f, 1.0f);
		return;
	}
	// Lower_Leg_L
	if (fs_JointID.x == 11.0f){
		o_FragColor = vec4(0.6f, 0.6f, 0.6f, 1.0f);
		return;
	}
	// Foot_L
	if (fs_JointID.x == 12.0f){
		o_FragColor = vec4(0.8f, 0.2f, 0.2f, 1.0f);
		return;
	}
	// Upper_Leg_R
	if (fs_JointID.x == 13.0f){
		o_FragColor = vec4(0.2f, 0.8f, 0.2f, 1.0f);
		return;
	}
	// Lower_Leg_R
	if (fs_JointID.x == 14.0f){
		o_FragColor = vec4(0.2f, 0.2f, 0.8f, 1.0f);
		return;
	}
	// Foot_R
	if (fs_JointID.x == 15.0f){
		o_FragColor = vec4(0.2f, 0.6f, 0.8f, 1.0f);
		return;
	}
	#endif

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