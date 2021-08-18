#version 330 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_TexCoords;
layout(location = 2) in vec3 a_Normals;
layout(location = 3) in vec3 a_Tangents;
layout(location = 4) in vec3 a_Bitangents;

// temporal
layout(location = 5) in vec3 a_JointID;
layout(location = 6) in vec3 a_Weights;

out vec2 fs_TexCoords;
out vec3 fs_ToLightVector;
out vec3 fs_ToCameraVector;

uniform mat4 u_Model;
uniform mat4 u_View;
uniform mat4 u_Projection;
uniform vec3 u_CameraPos;
uniform vec3 u_LightPos;
uniform vec4 u_Plane;

out vec3 fs_JointID;

uniform mat4 u_Joints[20];

void main() {

	fs_JointID = a_JointID;
	mat4 matrices[16];
	
	matrices[0] =  transpose(u_Joints[0]);
	matrices[1] =  transpose(u_Joints[1]); 
	matrices[2] =  mat4(0.999776, -0.00582242, 0.0203655, -0.0960783, 0.00729185, 0.997317, -0.0728393, 0.34138, -0.0198867, 0.0729715, 0.997135, 0.10203, 0, 0, 0, 1);
	matrices[3] =  mat4(0.999776, -0.00582243, 0.0203655, -0.0960783, 0.00729185, 0.997317, -0.0728393, 0.34138, -0.0198867, 0.0729715, 0.997135, 0.102031, 0, 0, 0, 1); 
	matrices[4] =  mat4(0.769379, 0.631079, 0.0989721, -0.390748, -0.490338, 0.682747, -0.541688, 3.39434, -0.409421, 0.368233, 0.834733, 1.30982, 0, 0, 0, 1);
	matrices[5] =  mat4(0.526908, 0.263403, 0.808076, -2.86597, -0.802277, 0.468003, 0.370574, 0.134793, -0.280572, -0.84356, 0.457916, 3.03477, 0, 0, 0, 1);
	matrices[6] =  mat4(0.565462, -0.102726, 0.818353, -3.00606, -0.824188, -0.0329384, 0.565359, -0.493703, -0.031121, -0.994164, -0.103291, 4.34381, 0, 0, 0, 1); 
	matrices[7] =  mat4(0.897435, -0.33486, -0.287191, 1.52532, 0.256678, 0.925828, -0.277415, 1.6951, 0.358785, 0.175246, 0.916822, 0.876892, 0, 0, 0, 1);
	matrices[8] =  mat4(0.627764, -0.401654, -0.666772, 2.6773, 0.714737, -0.0418624, 0.69814, -1.35352, -0.308323, -0.914834, 0.260797, 2.79263, 0, 0, 0, 1); 
	matrices[9] =  mat4(0.555176, 0.481533, -0.678164, 2.58166, 0.793577, -0.0625453, 0.605247, -0.855017, 0.249031, -0.874193, -0.416856, 6.3953, 0, 0, 0, 1); 
	matrices[10] = mat4(1, 1.17056e-07, -1.2786e-07, 4.17233e-07, 4.54311e-07, 0.975619, 0.219472, -0.703368, -6.2224e-08, -0.219473, 0.975618, 0.182899, 0, 0, 0, 1);
	matrices[11] = mat4(1, 4.64136e-08, -1.79747e-09, 3.57628e-07, 4.81897e-07, 0.997501, 0.0706632, -0.415018, -1.3033e-07, -0.0706633, 0.9975, 0.140346, 0, 0, 0, 1); 
	matrices[12] = mat4(1, -1.5034e-10, 9.44287e-08, 2.38419e-07, 5.60475e-07, 0.994637, 0.103437, -0.429444, -1.25471e-07, -0.103437, 0.994635, 0.147486, 0, 0, 0, 1);
	matrices[13] = mat4(1, -1.44006e-07, 6.11035e-08, -1.78814e-07, -4.10946e-07, 0.962173, 0.27244, -0.872333, 7.71543e-08, -0.272441, 0.962172, 0.231671, 0, 0, 0, 1); 
	matrices[14] = mat4(0.929486, 0.285367, 0.233714, -0.494204, 0.112822, 0.3833, -0.916706, 1.49828, -0.351179, 0.878438, 0.324073, 1.26283, 0, 0, 0, 1);
	matrices[15] = mat4(0.929165, 0.337948, 0.149815, -0.4655, 0.176934, -0.0507331, -0.982912, 1.64015, -0.324572, 0.9398, -0.106936, 1.46263, 0, 0, 0, 1); 
	
	mat4 bone_transform = u_Joints[int(a_JointID[0])] * a_Weights[0];
		 bone_transform += u_Joints[int(a_JointID[1])] * a_Weights[1];
		 bone_transform += u_Joints[int(a_JointID[2])] * a_Weights[2];
			
	vec4 posePosition = bone_transform * vec4(a_Pos, 1.0); // transformed by bones

	vec4 worldPos = u_Model * posePosition;
	gl_ClipDistance[0] = dot(worldPos, u_Plane);
	gl_Position = u_Projection * u_View * worldPos;

	fs_TexCoords = a_TexCoords;

	vec3 T = normalize(vec3(u_Model * vec4(a_Tangents, 0.0f)));
	vec3 B = normalize(vec3(u_Model * vec4(a_Bitangents, 0.0f)));
	vec3 N = normalize(vec3(u_Model * vec4(a_Normals, 0.0f)));
	mat3 TBN = transpose(mat3(T, B, N));

	fs_ToLightVector = normalize(TBN * (u_LightPos - worldPos.xyz));
	fs_ToCameraVector = normalize(TBN * (u_CameraPos - worldPos.xyz));

}