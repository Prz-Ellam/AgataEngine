#ifndef AGATA_GLM_UTILS_H
#define AGATA_GLM_UTILS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

glm::mat4 transformationMatrix(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale);

#endif