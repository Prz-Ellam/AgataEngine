#ifndef AGATA_FIRE_H
#define AGATA_FIRE_H

#include "Mesh.h"
#include "Texture2D.h"
#include "Shader.h"
#include <memory>

namespace Agata {

	class Fire {
	public:
		Fire(const std::string& rampPath, const std::string& alphaPath, const std::string& dudvPath, const std::string& noisePath,
			const glm::vec3& position, const glm::vec3& scale);
		~Fire();
		void draw(std::shared_ptr<Shader> shader, float dt);
		// void update(float ts);
	private:
		Mesh* m_Mesh;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::mat4 m_Transformation;
		Texture2D m_Ramp;
		Texture2D m_Alpha;
		Texture2D m_Dudv;
		Texture2D m_Noise;
		float m_Displacement;
	};

}

#endif