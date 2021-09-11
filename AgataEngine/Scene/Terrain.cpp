#include "Terrain.h"
#include <GL/glew.h>
#include <stb/stb_image.h>
#include "Renderer.h"
#include "glmUtils.h"
#include "Profiler.h"

namespace Agata {

	Terrain::Terrain(const std::string& textureRFilePath, const std::string& textureGFilePath, const std::string& textureBFilePath,
		const std::string& textureBGFilePath, const std::string& blendMapFilePath, const std::string& heightMapFilePath,
		const std::string& redNormal, const std::string& greenNormal, const std::string& blueNormal,
		const std::string& backgroundNormal, const glm::vec3& position, float height, float xSize, float zSize, float tilingFactor) :
		textureRed(textureRFilePath), textureGreen(textureGFilePath), textureBlue(textureBFilePath), textureBackground(textureBGFilePath),
		redNormal(redNormal, DefaultTex::NormalMap), blueNormal(blueNormal, DefaultTex::NormalMap),
		greenNormal(greenNormal, DefaultTex::NormalMap), backgroundNormal(backgroundNormal, DefaultTex::NormalMap),
		blendMap(blendMapFilePath), m_TilingFactor(tilingFactor), m_Position(position) {

		std::vector<Vertex3D> vertices;
		std::vector<uint32_t> indices;

		generateTerrain(vertices, indices, xSize, zSize, heightMapFilePath.c_str(), height);

		m_Mesh = new Mesh(vertices, indices);

	}

	//Terrain::Terrain(Terrain&& other) noexcept : textureRed(other.textureRed), textureGreen(other.textureBlue),
	//	textureBlue(other.textureBlue), textureBackground(other.textureBackground), blendMap(other.blendMap),
	//	redNormal(other.redNormal), blueNormal(other.blueNormal),
	//	greenNormal(other.greenNormal), backgroundNormal(other.backgroundNormal) {
	//
	//	this->m_Heights = other.m_Heights;
	//	this->m_Mesh = other.m_Mesh;
	//
	//	other.m_Mesh = nullptr;
	//
	//}

	Terrain::~Terrain() {

		delete m_Mesh;

	}

	void Terrain::generateTerrain(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices,
		uint32_t xSize, uint32_t zSize, const char* heightMap, int height) {

		bool defaultHeightMap = false;
		int length, width, bpp;
		stbi_set_flip_vertically_on_load(true);
		uint8_t* buffer = stbi_load(heightMap, &length, &width, &bpp, 1);

		if (!buffer) {
			buffer = new uint8_t;
			*buffer = 0;
			length = width = 1;
			defaultHeightMap = true;
		}

		std::vector<Vertex3D> verticesAux;
		std::vector<uint32_t> indicesAux;
		uint32_t count = length * width;

		m_GridSpacingX = xSize / ((float)length - 1);
		m_GridSpacingZ = zSize / ((float)width - 1);

		verticesAux.resize(count);
		indicesAux.resize(6ll * (length - 1ll) * (width - 1ll));
		m_Heights.resize(count);

		// Load coordinates, normals and texture coordinates
		for (int i = 0, z = 0; z < width; z++) {
			for (int x = 0; x < length; x++) {

				verticesAux[i].coords.x = x * m_GridSpacingX;
				verticesAux[i].coords.z = z * m_GridSpacingZ;

				verticesAux[i].coords.y = getHeight(x, z, length, width, height, buffer);
				m_Heights[i] = verticesAux[i].coords.y;
				
				verticesAux[i].normals = getNormals(x, z, length, width, height, buffer);

				verticesAux[i].uv.x = (float)x / (float)(length - 1);
				verticesAux[i].uv.y = (float)z / (float)(width - 1);

				i++;
			}
		}

		// Tangentes y Bitangentes
		for (int z = 0; z < width - 1; z++) {
			for (int x = 0; x < length - 1; x++) {

				int i0 = z * width + x;
				int i1 = (z + 1) * width + x;
				int i2 = z * width + (x + 1);

				glm::vec3 deltaPos1 = verticesAux[i1].coords - verticesAux[i0].coords;
				glm::vec3 deltaPos2 = verticesAux[i2].coords - verticesAux[i0].coords;

				glm::vec2 deltaUV1 = verticesAux[i1].uv - verticesAux[i0].uv;
				glm::vec2 deltaUV2 = verticesAux[i2].uv - verticesAux[i0].uv;

				float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

				glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
				glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

				verticesAux[i0].tangents = tangent;
				verticesAux[i1].tangents = tangent;
				verticesAux[i2].tangents = tangent;

				verticesAux[i0].bitangents = bitangent;
				verticesAux[i1].bitangents = bitangent;
				verticesAux[i2].bitangents = bitangent;

			}
		}

		// Load indices
		for (int i = 0, z = 0; z < width - 1; z++) {
			for (int x = 0; x < length - 1; x++) {
				indicesAux[i++] = z * width + x;			// Top Left
				indicesAux[i++] = (z + 1) * width + x;		// Bottom Left
				indicesAux[i++] = z * width + x + 1;		// Top Right
				indicesAux[i++] = z * width + x + 1;		// Top Right
				indicesAux[i++] = (z + 1) * width + x;		// Bottom Left
				indicesAux[i++] = (z + 1) * width + x + 1;	// Bottom Right
			}
		}

		if (buffer && !defaultHeightMap)
			stbi_image_free(buffer);
		else if (buffer && defaultHeightMap)
			delete buffer;

		vertices = verticesAux;
		indices = indicesAux;

	}

	float Terrain::getHeight(int x, int z, int length, int width, int height, uint8_t* data) {

		if (x < 0 || x > length || z < 0 || z > width) {
			return 0;
		}

		float result = data[x + z * width];
		result /= 255.0f;
		result *= height;
		return result;

	}

	glm::vec3 Terrain::getNormals(int x, int y, int length, int width, int height, uint8_t* data) {

		float left = getHeight(x - 1, y, length, width, height, data);
		float right = getHeight(x + 1, y, length, width, height, data);
		float top = getHeight(x, y + 1, length, width, height, data);
		float bottom = getHeight(x, y - 1, length, width, height, data);
		glm::vec3 normal = glm::vec3(left - right, 2.0f, bottom - top);
		return glm::normalize(normal);

	}

	void Terrain::draw(std::shared_ptr<Shader> shader, Light& light, const glm::vec4& clipDistance) {

		shader->bind();
		shader->sendMat4("u_Model", glm::translate(glm::mat4(1.0f), m_Position));
		shader->sendMat4("u_View", Renderer::getView());
		shader->sendMat4("u_Projection", Renderer::getProjection());
		shader->sendVec3("u_CameraPos", Renderer::getPosition());
		shader->sendVec3("u_LightPos", light.getPosition());
		shader->sendVec3("u_LightColour", light.getColour());
		shader->sendInt1("u_RedDiffuse", textureRed.bind(0));
		shader->sendInt1("u_GreenDiffuse", textureGreen.bind(1));
		shader->sendInt1("u_BlueDiffuse", textureBlue.bind(2));
		shader->sendInt1("u_Diffuse", textureBackground.bind(3));
		shader->sendInt1("u_RedNormal", redNormal.bind(4));
		shader->sendInt1("u_GreenNormal", greenNormal.bind(5));
		shader->sendInt1("u_BlueNormal", blueNormal.bind(6));
		shader->sendInt1("u_Normal", backgroundNormal.bind(7));
		shader->sendVec3("u_AmbientMaterial", m_Ambient);
		shader->sendVec3("u_DiffuseMaterial", m_Diffuse);
		shader->sendInt1("u_BlendMap", blendMap.bind(8));
		shader->sendFloat("u_TilingFactor", m_TilingFactor);
		shader->sendVec4("u_Plane", clipDistance);

		Renderer::drawIndex(m_Mesh->getVertexArray(), m_Mesh->getIndexCount());

	}

	float Terrain::getHeight(float x, float z) {

		//Agata::Profiler e(__FUNCTION__);
		float terrainX = x - m_Position.x;
		float terrainZ = z - m_Position.z;

		int gridX = ((int)(terrainX / m_GridSpacingX)) + 1;
		int gridZ = ((int)(terrainZ / m_GridSpacingZ)) + 1;

		if (gridX < 0 || gridZ < 0 || gridX > glm::sqrt(m_Heights.size()) - 1 || gridZ > glm::sqrt(m_Heights.size()) - 1) {
			return 0.0f;
		}

		float xCoord = (terrainX / m_GridSpacingX + 1) - gridX;
		float zCoord = (terrainZ / m_GridSpacingZ + 1) - gridZ;

		float height;
		if (xCoord <= (1 - zCoord)) {
			height = baryCentricCoordinatesY(glm::vec3(0, m_Heights[gridX + gridZ * 128ll], 0), // !!! Hardcode value
											glm::vec3(1, m_Heights[(gridX + 1ll) + gridZ * 128ll], 0),
											glm::vec3(0, m_Heights[gridX + (gridZ + 1ll) * 128ll], 1),
											glm::vec2(xCoord, zCoord));
		}
		else {
			height = baryCentricCoordinatesY(glm::vec3(1, m_Heights[(gridX + 1ll) + gridZ * 128ll], 0),
											glm::vec3(1, m_Heights[(gridX + 1ll) + (gridZ + 1ll) * 128ll], 1),
											glm::vec3(0, m_Heights[gridX + (gridZ + 1ll) * 128ll], 1),
											glm::vec2(xCoord, zCoord));
		}

		return height;

	}

	glm::vec3 Terrain::getAmbient() {

		return m_Ambient;

	}

	glm::vec3 Terrain::getDiffuse() {

		return m_Diffuse;

	}

	void Terrain::setAmbient(const glm::vec3& ambient) {

		m_Ambient = ambient;

	}

	void Terrain::setDiffuse(const glm::vec3& diffuse) {

		m_Diffuse = diffuse;

	}



	TerrainBuilder TerrainBuilder::GenerateParams() {

		return TerrainBuilder();

	}

	TerrainBuilder& TerrainBuilder::RedTexture(const std::string& filePath) {

		redDiffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::GreenTexture(const std::string& filePath) {

		greenDiffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlueTexture(const std::string& filePath) {

		blueDiffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlackTexture(const std::string& filePath) {

		diffuseTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Position(const glm::vec3& position) {

		this->position = position;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlendMap(const std::string& filePath) {

		blendMap = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::HeightMap(const std::string& filePath) {

		heightMap = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::RedNormal(const std::string& filePath) {

		redNormalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::GreenNormal(const std::string& filePath) {

		greenNormalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlueNormal(const std::string& filePath) {

		blueNormalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::BlackNormal(const std::string& filePath) {

		normalTex = filePath;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Width(float width) {

		this->width = width;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Depth(float depth) {

		this->depth = depth;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::Height(float height) {

		this->height = height;
		return *this;

	}

	TerrainBuilder& TerrainBuilder::TilingFactor(float tilingFactor) {

		this->tilingFactor = tilingFactor;
		return *this;

	}

	Terrain TerrainBuilder::Build() {

		return Terrain(redDiffuseTex, greenDiffuseTex, blueDiffuseTex, diffuseTex, blendMap, heightMap, redNormalTex, greenNormalTex,
			blueNormalTex, normalTex, position, height, width, depth, tilingFactor);

	}

	Terrain* TerrainBuilder::BuildNew() {

		return new Terrain(redDiffuseTex, greenDiffuseTex, blueDiffuseTex, diffuseTex, blendMap, heightMap, redNormalTex, greenNormalTex,
			blueNormalTex, normalTex, position, height, width, depth, tilingFactor);

	}

}