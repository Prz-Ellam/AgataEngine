#include "glmUtils.h"
#include "Profiler.h"

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
	exit[2][2] = scale.z * glm::cos(glm::radians(rot.z)) * glm::cos(glm::radians(rot.x));
	exit[2][3] = 0;
	exit[3][0] = pos.x;
	exit[3][1] = pos.y;
	exit[3][2] = pos.z;
	exit[3][3] = 1;
	return exit;

}

glm::mat4 transformationMatrix(const glm::vec3& pos, const glm::quat& quat, const glm::vec3& scale) {

	glm::mat4 exit;
	exit[0][0] = scale.x * (1 - 2 * quat.y * quat.y - 2 * quat.z * quat.z);
	exit[0][1] = scale.x * (2 * quat.y * quat.x + 2 * quat.z * quat.w);
	exit[0][2] = scale.x * (2 * quat.z * quat.x - 2 * quat.y * quat.w);
	exit[0][3] = 0;
	exit[1][0] = scale.y * (2 * quat.y * quat.x - 2 * quat.z * quat.w);
	exit[1][1] = scale.y * (1 - 2 * quat.z * quat.z - 2 * quat.x * quat.x);
	exit[1][2] = scale.y * (2 * quat.y * quat.z + 2 * quat.x * quat.w);
	exit[1][3] = 0;
	exit[2][0] = scale.z * (2 * quat.y * quat.w + 2 * quat.z * quat.x);
	exit[2][1] = scale.z * (2 * quat.y * quat.z - 2 * quat.x * quat.w);
	exit[2][2] = scale.z * (1 - 2 * quat.y * quat.y - 2 * quat.x * quat.x);
	exit[2][3] = 0;
	exit[3][0] = pos.x;
	exit[3][1] = pos.y;
	exit[3][2] = pos.z;
	exit[3][3] = 1;
	return exit;

}

float baryCentricCoordinatesY(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos) {

	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;

}