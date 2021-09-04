#include "GrassManager.h"
#include "Renderer.h"
#include "Loader.h"
#include "glmUtils.h"
#include "Random.h"

namespace Agata {

	GrassManager::GrassManager(uint32_t maxGrassCount) {

		m_VAO = new VertexArray();

		m_VBO = new VertexBuffer(maxGrassCount * 12 * sizeof(GrassVertex));
		m_IBO = new IndexBuffer(maxGrassCount * 18 * sizeof(uint32_t));

		std::vector<BufferLayout> layouts;
		layouts.reserve(6);
		layouts.emplace_back(0, DataType::Float3); // Position
		layouts.emplace_back(1, DataType::Float2); // Texture Coordinates
		layouts.emplace_back(2, DataType::Float3); // Normals
		layouts.emplace_back(3, DataType::Float3); // Tangents
		layouts.emplace_back(4, DataType::Float3); // Bitangents
		layouts.emplace_back(5, DataType::Float);  // Texture ID
		layouts.emplace_back(6, DataType::Float);  // Displacement Factor

		m_VAO->attachVertexBuffer(m_VBO, layouts);
		m_VAO->attachIndexBuffer(m_IBO);

		//m_Textures.reserve(32);

	}

	GrassManager::~GrassManager() {

		delete m_VAO;
		delete m_VBO;
		delete m_IBO;

	}

	void GrassManager::addGrassUnit(const glm::vec3& position, const glm::vec3& scale, uint32_t texIndex) {

		auto [vertices, indices] = Loader::loadGrass();

		int random = Random::Int(0, 359);
		for (GrassVertex& vertex : vertices) {
			vertex.coords = transformationMatrix(position, glm::vec3(1.0f), scale) * glm::vec4(vertex.coords, 1.0f);
			//vertex.normals = transformationMatrix(position, glm::vec3(1.0f), scale) * glm::vec4(vertex.normals, 1.0f);
			vertex.texIndex = texIndex;
			vertex.displacement = random;
		}

		for (uint32_t& index : indices) {
			index = index + (m_IndexCount / 18) * vertices.size();
		}

		m_VBO->sendData(vertices.data(), vertices.size() * sizeof(GrassVertex), m_VertexOffset);
		m_VertexOffset += vertices.size() * sizeof(GrassVertex);
		m_IBO->sendData(indices.data(), indices.size() * sizeof(uint32_t), m_IndicesOffset);
		m_IndicesOffset += indices.size() * sizeof(uint32_t);
		m_IndexCount += indices.size();

	}

	void GrassManager::batchGrassUnits() {

	
		
	}

	void GrassManager::addTexture(const std::string& filePath) {

		m_Textures.emplace_back(filePath);

	}

	using namespace std::literals::string_literals;

	void GrassManager::draw(std::shared_ptr<Shader> shader, Light& light, float dt) {

		(m_Time < 360.0f) ? m_Time += 1.0f * dt : m_Time = 0.0f;

		glDisable(GL_CULL_FACE);
		shader->bind();
		shader->sendMat4("u_Projection", Renderer::getProjection());
		shader->sendMat4("u_View", Renderer::getView());
		shader->sendVec3("u_CameraPos", Renderer::getPosition());
		shader->sendVec3("u_LightPos", light.getPosition());
		shader->sendVec4("u_LightColour", glm::vec4(light.getColour(), 1.0f));
		shader->sendFloat("u_Time", m_Time);

		for (int i = 0; i < m_Textures.size(); i++) {
			std::string e = "u_Textures[" + std::to_string(i) + "]"s;
			shader->sendInt1(e, m_Textures[i].bind(i));
		}

		Renderer::drawIndex(m_VAO, m_IndexCount);
		glEnable(GL_CULL_FACE);

	}

}