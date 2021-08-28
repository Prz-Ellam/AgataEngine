#include "Zoom.h"
#include "Loader.h"
#include "Renderer.h"

namespace Agata {

	Zoom::Zoom(const std::string& shape) : m_Shape(shape) {

		auto [vertices, indices] = Loader::loadQuad();

		m_Mesh = new Mesh(vertices, indices);

	}

	Zoom::~Zoom() {

		delete m_Mesh;

	}

	void Zoom::draw(std::shared_ptr<Shader> shader) {

		shader->bind();
		shader->sendInt1("u_Shape", m_Shape.bind(0));
		Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());

	}

}