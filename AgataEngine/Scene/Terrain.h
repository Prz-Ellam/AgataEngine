#ifndef AGATA_TERRAIN_H
#define AGATA_TERRAIN_H

#include "Mesh.h"
#include "Texture2D.h"
#include "Shader.h"
#include "Light.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

namespace Agata {

	class Terrain {
	private:
		Mesh* m_Mesh;
		Texture2D textureRed;
		Texture2D textureGreen;
		Texture2D textureBlue;
		Texture2D textureBackground;
		Texture2D redNormal;
		Texture2D greenNormal;
		Texture2D blueNormal;
		Texture2D backgroundNormal;
		Texture2D blendMap;
		glm::vec3 m_Ambient;
		glm::vec3 m_Diffuse;

		float m_GridSpacingX, m_GridSpacingZ;

		std::vector<float> m_Heights;
		float m_TilingFactor;
		void generateTerrain(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices,
			uint32_t xSize, uint32_t zSize, const char* heightMap, int height);
		float getHeight(int x, int z, int length, int width, int height, uint8_t* data);
		glm::vec3 getNormals(int x, int y, int length, int width, int height, uint8_t* data);
	public:
		Terrain(const std::string& textureRFilePath, const std::string& textureGFilePath, const std::string& textureBFilePath,
			const std::string& textureBGFilePath,
			const std::string& redNormal, const std::string& greenNormal, const std::string& blueNormal,
			const std::string& backgroundNormal,
			const std::string& blendMapFilePath, const std::string& heightMapFilePath,
			float tilingFactor);

		//Terrain(Terrain&& other) noexcept;

		glm::vec3 getAmbient();
		glm::vec3 getDiffuse();

		void setAmbient(const glm::vec3& ambient);
		void setDiffuse(const glm::vec3& diffuse);


		float& getTilingFactorRef() { return m_TilingFactor; }

		virtual ~Terrain();
		void draw(std::shared_ptr<Shader> shader, Light& light, const glm::vec4& clipDistance = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
		float getHeight(float x, float z);
	};


	class TerrainBuilder {
	public:
		TerrainBuilder() = default;
		TerrainBuilder& RedTexture(const std::string& filePath);
		TerrainBuilder& GreenTexture(const std::string& filePath);
		TerrainBuilder& BlueTexture(const std::string& filePath);
		TerrainBuilder& BlackTexture(const std::string& filePath);

		TerrainBuilder& RedNormal(const std::string& filePath);
		TerrainBuilder& GreenNormal(const std::string& filePath);
		TerrainBuilder& BlueNormal(const std::string& filePath);
		TerrainBuilder& BlackNormal(const std::string& filePath);

		TerrainBuilder& BlendMap(const std::string& filePath);
		TerrainBuilder& HeightMap(const std::string& filePath);
		TerrainBuilder& TilingFactor(float tilingFactor);
		Terrain Build();
		Terrain* BuildHeap();
	private:
		std::string diffuseTex;
		std::string normalTex;
		std::string redDiffuseTex;
		std::string redNormalTex;
		std::string greenDiffuseTex;
		std::string greenNormalTex;
		std::string blueDiffuseTex;
		std::string blueNormalTex;
		std::string blendMap;
		std::string heightMap;
		float tilingFactor = 1.0f;
	};

}


#endif