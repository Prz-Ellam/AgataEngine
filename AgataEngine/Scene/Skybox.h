#ifndef SKYBOX_H_
#define SKYBOX_H_

#include "Mesh.h"
#include "Texture3D.h"
#include <string>
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace Agata {

	class Skybox {
	public:
		Skybox(const std::string& right, const std::string& left, const std::string& top, const std::string& bottom,
			const std::string& front, const std::string& back, float size);
		// 6 textures morning
		// 6 textures day
		// 6 textures afternoon
		// 6 textures night
		// size

		virtual ~Skybox();
		void draw(std::shared_ptr<Shader> shader, Light& light);

		float getRotation() const;
		void setRotation(float rotation);
		void updateRotation(float rotation);

	private:
		Texture3D m_Texture;
		Mesh* m_Mesh;
		// Texture3D m_Texture;
		float m_Rotation;
		float m_BlendFactor;
	};

}

#endif