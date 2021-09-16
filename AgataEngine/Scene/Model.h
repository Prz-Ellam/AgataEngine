#ifndef MODEL_H
#define MODEL_H

#include <string>
#include "Mesh.h"
#include "Material.h"
#include "Shader.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace Agata {

	class Model {
	public:
		friend class ModelBuilder;
		// Model(const std::string& filePath);
		Model(const std::string& filePath, const std::string& diffuseTex, const std::string& specularTex, const std::string& normalTex,
			const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,
			const glm::vec3& ambientMaterial, const glm::vec3& diffuseMaterial, const glm::vec3& specularMaterial,
			float shininessMaterial);

		~Model();
		glm::mat4 getTransformation() const;
		void setPosition(const glm::vec3& position);

		void setRotation(const glm::vec3& rotation);

		Material& getMaterialRef() { return m_Material; }

		void draw();
		void draw(std::shared_ptr<Shader> shader, Light& light, const glm::vec4& clipDistance = glm::vec4(0.0f));
	private:
		Mesh* m_Mesh;
		Material m_Material;
		glm::vec4 m_Colour;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_Transformation;
	};

	class ModelBuilder {
	public:
		ModelBuilder() = default;

		static ModelBuilder GenerateParams();

		ModelBuilder& ModelPath(const std::string&);
		ModelBuilder& DiffuseTexture(const std::string&);
		ModelBuilder& SpecularTexture(const std::string&);
		ModelBuilder& NormalTexture(const std::string&);
		ModelBuilder& Position(const glm::vec3&);
		ModelBuilder& Rotation(const glm::vec3&);
		ModelBuilder& Scale(const glm::vec3&);
		ModelBuilder& AmbientMaterial(const glm::vec3&);
		ModelBuilder& DiffuseMaterial(const glm::vec3&);
		ModelBuilder& SpecularMaterial(const glm::vec3&);
		ModelBuilder& ShininessMaterial(float);
		Model Build();
		Model* BuildHeap();
	private:
		std::string modelFilePath;
		std::string diffuseTexture;
		std::string specularTexture;
		std::string normalTexture;
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 scale = glm::vec3(1.0f);
		glm::vec3 ambientMaterial = glm::vec3(0.15f);
		glm::vec3 diffuseMaterial = glm::vec3(1.0f);
		glm::vec3 specularMaterial = glm::vec3(1.0f);
		float shininessMaterial = 32;
	};

}

#endif