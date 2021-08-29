#ifndef AGATA_GLM_UTILS_H
#define AGATA_GLM_UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Profiler.h"

glm::mat4 transformationMatrix(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
glm::mat4 transformationMatrix(const glm::vec3& pos, const glm::quat& quat, const glm::vec3& scale);

float baryCentricCoordinatesY(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);

inline glm::mat4 assimpToGlmMatrix(const aiMatrix4x4& mat) {

	return glm::mat4(mat[0][0], mat[1][0], mat[2][0], mat[3][0], 
					 mat[0][1], mat[1][1], mat[2][1], mat[3][1], 
					 mat[0][2], mat[1][2], mat[2][2], mat[3][2],
					 mat[0][3], mat[1][3], mat[2][3], mat[3][3]
	);

}

inline glm::vec3 assimpToGlmVec3(const aiVector3D& v) {

	return glm::vec3(v.x, v.y, v.z);

}

inline glm::quat assimpToGlmQuat(const aiQuaternion& q) {

	return glm::quat(q.w, q.x, q.y, q.z);

}

#endif