#include <GL/glew.h>
#include "Water.h"
#include "Loader.h"
#include "Renderer.h"

namespace Agata {

	Water::Water() : m_Position(glm::vec3(0.0f)), m_Scale(glm::vec3(1.0f)), dudvMap(""), normalMap(""), m_Refraction(480, 360),
		m_Reflection(480, 360) {

		auto [vertices, indices] = Loader::loadHorizontalQuad();

		m_Mesh = new Mesh(vertices, indices);

		m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
		m_Transformation = glm::scale(m_Transformation, m_Scale);

	}

	Water::Water(const glm::vec3& position, const glm::vec3& scale, const std::string& dudvMap, const std::string& normalMap,
		uint32_t width, uint32_t height)
		: dudvMap(dudvMap), normalMap(normalMap, DefaultTex::NormalMap), m_Refraction(width, height), m_Reflection(width, height),
		m_Width(width), m_Height(height) {

		auto [vertices, indices] = Loader::loadHorizontalQuad();

		m_Mesh = new Mesh(vertices, indices);

		m_Position = position;
		m_Scale = scale;

		m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
		m_Transformation = glm::scale(m_Transformation, m_Scale);

	}

	Water::~Water() {

		delete m_Mesh;

	}

	void Water::startReflection() {

		m_Reflection.bind(m_Width, m_Height);

	}

	void Water::endReflection() {

		m_Reflection.unbind();

	}

	void Water::startRefraction() {

		m_Refraction.bind(m_Width, m_Height);

	}

	void Water::endRefraction() {

		m_Refraction.unbind();

	}

	glm::vec3& Water::getPositionRef() {

		return m_Position;

	}

	glm::vec3& Water::getScaleRef() {

		return m_Scale;

	}

	float Water::getHeight() const {

		return m_Position.y;

	}

	void Water::draw(std::shared_ptr<Shader> shader, Light& light) {

		//m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
		//m_Transformation = glm::scale(m_Transformation, m_Scale);

		moveFactor += 0.0003f;
		if (moveFactor > 1)
			moveFactor = 0;

		glCullFace(GL_FRONT);
		shader->bind();
		shader->sendMat4("u_Projection", Renderer::getProjection());
		shader->sendMat4("u_View", Renderer::getView());
		shader->sendMat4("u_Model", m_Transformation);
		shader->sendVec3("u_CameraPosition", Renderer::getPosition());
		shader->sendVec3("u_LightPosition", light.getPosition());
		shader->sendVec3("u_LightColour", light.getColour());
		shader->sendInt1("u_Refraction", m_Refraction.bindColorTexture(0));
		shader->sendInt1("u_Reflection", m_Reflection.bindColorTexture(1));
		shader->sendInt1("u_dudv", dudvMap.bind(2));
		shader->sendInt1("u_NormalMap", normalMap.bind(3));
		shader->sendInt1("u_DepthMap", m_Refraction.bindDepthTexture(4));
		shader->sendFloat("u_MoveFactor", moveFactor);

		Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());
		glCullFace(GL_BACK);

	}

}