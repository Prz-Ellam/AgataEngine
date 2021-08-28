#ifndef AGATA_WATER_H
#define AGATA_WATER_H

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Light.h"
#include "Texture2D.h"
#include "FrameBuffer.h"
#include <memory>

namespace Agata {

	class Water {
	public:
		Water();
		Water(const glm::vec3& position, const glm::vec3& scale, const std::string& dudvMap, const std::string& normalMap,
			uint32_t width, uint32_t height);
		~Water();

		glm::vec3& getPositionRef();
		glm::vec3& getScaleRef();

		void startReflection();
		void endReflection();

		void startRefraction();
		void endRefraction();

		float getHeight() const;

		void draw(std::shared_ptr<Shader> shader, Light& light);
	private:
		Mesh* m_Mesh;
		glm::vec4 m_Colour;
		Texture2D dudvMap;
		Texture2D normalMap;

		FrameBuffer m_Refraction;
		FrameBuffer m_Reflection;

		uint32_t m_Width, m_Height;

		float moveFactor = 0.0f;
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::mat4 m_Transformation;
	};

}

#endif