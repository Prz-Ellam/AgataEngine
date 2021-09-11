#ifndef AGATA_GRASS_MANAGER_H
#define AGATA_GRASS_MANAGER_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Light.h"
#include <memory>
#include "Texture2D.h"

namespace Agata {

	// Max Grass Count
	// Max Textures
	// Max Vertex and Index

	class GrassManager {
	public:
		GrassManager(uint32_t maxGrassCount);
		~GrassManager();

		void addGrassUnit(const glm::vec3& position, const glm::vec3& scale, uint32_t texIndex);
		void addTexture(const std::string& filePath);


		// Load all the textures
		// Load all the grass and ferns with positions and scales

		void draw(std::shared_ptr<Shader> shader, Light& light, float dt);

	private:
		// 12 Vertex - 18 Index
		uint32_t m_VertexOffset;
		uint32_t m_IndicesOffset;
		float m_Time;

		VertexArray* m_VAO;
		VertexBuffer* m_VBO;
		IndexBuffer* m_IBO;
		uint32_t m_IndexCount;

		std::vector<Texture2D> m_Textures;
	};

}

#endif