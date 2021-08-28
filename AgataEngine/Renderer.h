#ifndef AGATA_RENDERER_H
#define AGATA_RENDERER_H

#include "Terrain.h"
#include "Skybox.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Camera.h"
#include <GLFW\glfw3.h>
#include "Window.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Window.h"

namespace Agata {

	class Renderer {
	public:
		Renderer();
		~Renderer();
		void backCullface();
		void viewport(Window* window);
		void clear(float r, float g, float b, float a);

		static glm::mat4 getView();
		static glm::mat4 getProjection();
		static glm::vec3 getPosition();

		void beginScene(Camera* camera);
		void endScene();

		static void drawIndex(VertexArray* vao, uint32_t indexCount);
		static void drawArray(VertexArray* vao, uint32_t arrayCount);
	private:
		static glm::mat4 s_Projection;
		static glm::mat4 s_View;
		static glm::vec3 s_Position;
	};

}

#endif