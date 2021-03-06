#ifndef AGATA_BILLBOARD_H
#define AGATA_BILLBOARD_H

#include <string>
#include "Texture2D.h"
#include "Mesh.h"
#include "Shader.h"
#include "Light.h"
#include "Camera.h"
#include <memory>

namespace Agata {

	class Billboard {
	public:
		Billboard(const std::string& filePath);
		Billboard(const std::string& filePath, const glm::vec3& position, const glm::vec3& scale);
		~Billboard();

		glm::vec3& getPositionRef() { return m_Position; }
		void draw(std::shared_ptr<Shader> shader, Light& light, float dt);
	private:
		Mesh* m_Mesh;
		Texture2D m_Texture;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_Transformation;
		float time = 0.0f;
	};

}

#endif