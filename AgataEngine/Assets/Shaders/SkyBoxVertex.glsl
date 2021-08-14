#version 330 core

layout(location = 0) in vec3 a_Pos;

out vec3 fs_TexCoords;

uniform mat4 u_View;
uniform mat4 u_Projection;

void main() {

	vec4 ndcPos = u_Projection * u_View * vec4(a_Pos, 1.0f);
	gl_Position = vec4(ndcPos.x, ndcPos.y, ndcPos.w, ndcPos.w);

	fs_TexCoords = vec3(a_Pos.x, a_Pos.y, -a_Pos.z);

}