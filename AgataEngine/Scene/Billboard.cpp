#include <GL/glew.h>
#include "Billboard.h"
#include "Loader.h"
#include "Renderer.h"
#include <cmath>

namespace Agata {

	Billboard::Billboard(const std::string& filePath) : m_Texture(filePath) {

		auto [vertices, indices] = Loader::loadQuad();

		m_Mesh = new Mesh(vertices, indices);

	}

	Billboard::Billboard(const std::string& filePath, const glm::vec3& position, const glm::vec3& scale) : m_Texture(filePath),
		m_Position(position), m_Scale(scale) {

		auto [vertices, indices] = Loader::loadGrass();

		m_Mesh = new Mesh(vertices, indices);

		m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
		m_Transformation = glm::scale(m_Transformation, m_Scale);

	}

	Billboard::~Billboard() {

		delete m_Mesh;

	}

	void Billboard::draw(std::shared_ptr<Shader> shader, Light& light, float dt) {
;
		(time < 360.0f) ? time += 1.0f * dt : time = 0.0f;

		//glm::vec3 vecPos = m_Position - Renderer::getPosition();
		//float angle = std::atan2(vecPos.x, vecPos.z);
		//
		//m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
		//m_Transformation = glm::rotate(m_Transformation, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		//m_Transformation = glm::scale(m_Transformation, m_Scale);

		glDisable(GL_CULL_FACE);
		shader->bind();
		shader->sendMat4("u_Projection", Renderer::getProjection());
		shader->sendMat4("u_View", Renderer::getView());
		shader->sendMat4("u_Model", m_Transformation);
		shader->sendInt1("u_Texture", m_Texture.bind(0));
		shader->sendVec3("u_CameraPos", Renderer::getPosition());
		shader->sendVec3("u_LightPos", light.getPosition());
		shader->sendVec4("u_LightColour", glm::vec4(light.getColour(), 1.0f));
		shader->sendFloat("u_Time", time);

		Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());
		glEnable(GL_CULL_FACE);

	}

}