#include "Fire.h"
#include "Loader.h"
#include "Renderer.h"
#include "glmUtils.h"
#include "Profiler.h"

namespace Agata {

	Fire::Fire(const std::string& rampPath, const std::string& alphaPath, const std::string& dudvPath, const std::string& noisePath,
		const glm::vec3& position, const glm::vec3& scale) 
		: m_Ramp(rampPath), m_Alpha(alphaPath), m_Dudv(dudvPath), m_Noise(noisePath), m_Position(position), m_Scale(scale) {

		auto [vertices, indices] = Loader::loadQuad();
		m_Mesh = new Mesh(vertices, indices);

		m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
		m_Transformation = glm::scale(m_Transformation, m_Scale);

	}

	Fire::~Fire() {

		delete m_Mesh;

	}

	void Fire::draw(std::shared_ptr<Shader> shader, float dt) {

		glm::vec3 vecPos = m_Position - Renderer::getPosition();
		float angle = std::atan2(vecPos.x, vecPos.z);

		//m_Transformation = glm::translate(glm::mat4(1.0f), m_Position);
		//m_Transformation = glm::rotate(m_Transformation, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		//m_Transformation = glm::scale(m_Transformation, m_Scale);

		m_Transformation = transformationMatrix(m_Position,glm::vec3(0.0f, glm::degrees(angle), 0.0f), m_Scale);
		(m_Displacement < 1.0f) ? m_Displacement += 0.05f * dt : m_Displacement = 0.0f;

		glDisable(GL_CULL_FACE);

		shader->bind();
		shader->sendMat4("u_Projection", Renderer::getProjection());
		shader->sendMat4("u_View", Renderer::getView());
		shader->sendMat4("u_Model", m_Transformation);
		shader->sendInt1("u_Noise", m_Noise.bind(0));
		shader->sendInt1("u_Alpha", m_Alpha.bind(1));
		shader->sendInt1("u_Ramp", m_Ramp.bind(2));
		shader->sendFloat("u_dudv", m_Dudv.bind(3));
		shader->sendVec3("u_CameraPos", Renderer::getPosition());
		shader->sendFloat("u_Displacement", m_Displacement);

		Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());
		glEnable(GL_CULL_FACE);

	}


}