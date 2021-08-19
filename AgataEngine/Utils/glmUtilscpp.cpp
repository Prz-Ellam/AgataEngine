#include "glmUtils.h"

glm::mat4 transformationMatrix(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) {

	glm::mat4 exit;
	exit[0][0] = scale.x * glm::cos(glm::radians(rot.y)) * glm::cos(glm::radians(rot.z));
	exit[0][1] = scale.x * (glm::sin(glm::radians(rot.x)) * glm::sin(glm::radians(rot.y)) * glm::cos(glm::radians(rot.z)) + glm::cos(glm::radians(rot.x)) * glm::sin(glm::radians(rot.z)));
	exit[0][2] = scale.x * (glm::sin(glm::radians(rot.x)) * glm::sin(glm::radians(rot.z)) - glm::cos(glm::radians(rot.x)) * glm::sin(glm::radians(rot.y)) * glm::cos(glm::radians(rot.z)));
	exit[0][3] = 0;
	exit[1][0] = -scale.y * glm::cos(glm::radians(rot.y)) * glm::sin(glm::radians(rot.z));
	exit[1][1] = scale.y * (glm::cos(glm::radians(rot.x)) * glm::cos(glm::radians(rot.z)) - glm::sin(glm::radians(rot.x)) * glm::sin(glm::radians(rot.y)) * glm::sin(glm::radians(rot.z)));
	exit[1][2] = scale.y * (glm::cos(glm::radians(rot.x)) * glm::sin(glm::radians(rot.y)) * glm::sin(glm::radians(rot.z)) + glm::sin(glm::radians(rot.x)) * glm::cos(glm::radians(rot.z)));
	exit[1][3] = 0;
	exit[2][0] = scale.z * glm::sin(glm::radians(rot.y));
	exit[2][1] = -scale.z * glm::sin(glm::radians(rot.x)) * glm::cos(glm::radians(rot.y));
	exit[2][2] = scale.z * glm::cos(glm::radians(rot.z)) * glm::cos(glm::radians(rot.y));
	exit[2][3] = 0;
	exit[3][0] = pos.x;
	exit[3][1] = pos.y;
	exit[3][2] = pos.z;
	exit[3][3] = 1;
	return exit;

}