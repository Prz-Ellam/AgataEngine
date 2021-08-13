#include <GL/glew.h>
#include "Renderer.h"

glm::mat4 Renderer::s_Projection = glm::mat4(1.0f);
glm::mat4 Renderer::s_View = glm::mat4(1.0f);
glm::vec3 Renderer::s_Position = glm::vec3(1.0f);

Renderer::Renderer() {
}

Renderer::~Renderer() {

}

void Renderer::clear(float r, float g, float b, float a) {

	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Renderer::backCullface() {

	glEnable(GL_CULL_FACE);	// Para que no se renderizan las caras internas del modelo (nunca se van a ver en escena)
	glCullFace(GL_BACK);

}

void Renderer::viewport(Window* window) {

	int width, height;
	glfwGetWindowSize(window->getHandler(), &width, &height);
	glViewport(0, 0, width, height);

}

void Renderer::drawIndex(VertexArray* vao, uint32_t indexCount) {

	vao->bind();
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

}

void Renderer::drawArray(VertexArray* vao, uint32_t arrayCount) {

	vao->bind();
	glDrawArrays(GL_TRIANGLES, 0, arrayCount);

}

glm::mat4 Renderer::getView() {

	return s_View;

}

glm::mat4 Renderer::getProjection() {

	return s_Projection;

}

glm::vec3 Renderer::getPosition() {

	return s_Position;

}

void Renderer::beginScene(Camera* camera) {

	s_View = camera->getView();
	s_Projection = camera->getProjection();
	s_Position = camera->getPosition();

}

void Renderer::endScene() {

}