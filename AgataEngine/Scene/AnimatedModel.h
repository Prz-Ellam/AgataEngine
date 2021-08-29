#ifndef AGATA_ANIMATED_MODEL_H
#define AGATA_ANIMATED_MODEL_H

#include "Mesh.h"
#include "Material.h"
#include "Joint.h"
#include "Light.h"
#include "Animation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Agata {

	class AnimatedModel {
	public:
		AnimatedModel(const std::string& filePath, const std::string& diffuseTex, const std::string& specularTex, const std::string& normalTex,
			const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,
			const glm::vec3& ambientMaterial, const glm::vec3& diffuseMaterial, const glm::vec3& specularMaterial,
			float shininessMaterial);

		glm::mat4 getTransformation() const;
		void setPosition(const glm::vec3& position);
		void setRotation(const glm::vec3& rotation);

		Material& getMaterialRef() { return m_Material; }

		void draw(std::shared_ptr<Shader> shader, Light& light, float ts, const glm::vec4& clipDistance = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));

	private:
		Material m_Material;
		//Joint m_RootJoint;
		uint32_t m_JointCount;

		glm::mat4 m_GlobalInverseTransformation;

		Assimp::Importer importer;
		const aiScene* scene;
		std::vector<Mesh> meshes;
		std::map<std::string, uint32_t> jointInfo;
		std::vector< std::pair<glm::mat4, glm::mat4>> jointTransformations;
		std::map<std::string, const aiNodeAnim*> animationInfo;
		float ticksPerSec;

		void loadModel(const std::string& path);
		void processNode(aiNode* node, const aiScene* scene);
		void processMesh(aiMesh* mesh, const aiScene* scene, std::vector<AnimVertex>& vertices, std::vector<uint32_t>& indices);
		void loadJointsTransforms(float timeStamp, std::vector<glm::mat4>& transformations);
		void findTransformations(float timeStamp, const aiNode* node, const glm::mat4 parentTransformation = glm::mat4(1.0f));
		glm::vec3 lerpPosition(float animationTime, const aiNodeAnim* node);
		glm::quat lerpRotation(float animationTime, const aiNodeAnim* node);
		glm::vec3 lerpScale(float animationTime, const aiNodeAnim* node);

		glm::vec4 m_Colour;
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		glm::vec3 m_Scale;
		glm::mat4 m_Transformation;
	};

	class AnimatedModelBuilder {
	public:
		AnimatedModelBuilder() = default;
		AnimatedModelBuilder& ModelPath(const std::string&);
		AnimatedModelBuilder& DiffuseTexture(const std::string&);
		AnimatedModelBuilder& SpecularTexture(const std::string&);
		AnimatedModelBuilder& NormalTexture(const std::string&);
		AnimatedModelBuilder& Position(const glm::vec3&);
		AnimatedModelBuilder& Rotation(const glm::vec3&);
		AnimatedModelBuilder& Scale(const glm::vec3&);
		AnimatedModelBuilder& AmbientMaterial(const glm::vec3&);
		AnimatedModelBuilder& DiffuseMaterial(const glm::vec3&);
		AnimatedModelBuilder& SpecularMaterial(const glm::vec3&);
		AnimatedModelBuilder& ShininessMaterial(float);
		AnimatedModel Build();
		AnimatedModel* BuildHeap();
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