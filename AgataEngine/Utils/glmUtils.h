#ifndef AGATA_GLM_UTILS_H
#define AGATA_GLM_UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

glm::mat4 transformationMatrix(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);
glm::mat4 assimpToGlmMatrix(aiMatrix4x4 mat);

inline glm::vec3 assimpToGlmVec3(const aiVector3D& v) {

	return glm::vec3(v.x, v.y, v.z);

}

inline glm::quat assimpToGlmQuat(const aiQuaternion& q) {

	return glm::quat(q.w, q.x, q.y, q.z);

}

#endif