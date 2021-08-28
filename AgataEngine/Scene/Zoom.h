#ifndef AGATA_ZOOM_H
#define AGATA_ZOOM_H

#include <string>
#include "Mesh.h"
#include "Texture2D.h"
#include "Shader.h"
#include <memory>

namespace Agata {

	class Zoom {
	public:
		Zoom(const std::string& shape);
		~Zoom();
		void draw(std::shared_ptr<Shader> shader);
	private:
		Mesh* m_Mesh;
		Texture2D m_Shape;
	};

}

#endif