#include <GL/glew.h>
#include "Skybox.h"
#include "Loader.h"
#include "Renderer.h"
#include "Timer.h"

Skybox::Skybox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom,
	const std::string& front, const std::string& back, float size) : m_Texture(right, left, top, bottom, front, back) {

	auto [vertices, indices] = Loader::loadSkyBox(size);

	m_Mesh = new Mesh(vertices, indices);
	m_Rotation = 0.0f;

}

Skybox::~Skybox() {

	delete m_Mesh;

}

void Skybox::draw(std::shared_ptr<Shader> shader, Light& light) {

	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);

	shader->bind();
	shader->sendInt1("u_CubeMap", m_Texture.bind(0));
	shader->sendMat4("u_View", glm::rotate(Renderer::getView(), glm::radians(m_Rotation), glm::vec3(0, 1, 0)));
	shader->sendMat4("u_Projection", Renderer::getProjection());
	Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());

	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

}

float Skybox::getRotation() { 
	
	return m_Rotation;

}

void Skybox::setRotation(float rotation) {

	m_Rotation = rotation;
	
}

void Skybox::updateRotation(float rotation) {
	
	(m_Rotation >= 360.0f) ? m_Rotation = 0.0f : m_Rotation += rotation;

}