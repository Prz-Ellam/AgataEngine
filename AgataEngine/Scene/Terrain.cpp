#include "Terrain.h"
#include <GL/glew.h>
#include <stb/stb_image.h>
#include "Renderer.h"
#include <iostream>

Terrain::Terrain(const std::string& textureRFilePath, const std::string& textureGFilePath, const std::string& textureBFilePath,
	const std::string& textureBGFilePath, const std::string& blendMapFilePath, const std::string& heightMapFilePath,
	const std::string& redNormal, const std::string& greenNormal, const std::string& blueNormal,
	const std::string& backgroundNormal, float tilingFactor) :
	textureRed(textureRFilePath), textureGreen(textureGFilePath), textureBlue(textureBFilePath), textureBackground(textureBGFilePath), 
	redNormal(redNormal, DefaultTex::NormalMap), blueNormal(blueNormal, DefaultTex::NormalMap),
	greenNormal(greenNormal, DefaultTex::NormalMap), backgroundNormal(backgroundNormal, DefaultTex::NormalMap),
	blendMap(blendMapFilePath), m_TilingFactor(tilingFactor) {

	std::vector<Vertex3D> vertices;
	std::vector<uint32_t> indices;

	generateTerrain(vertices, indices, 20, 20, heightMapFilePath.c_str(), 6);

	m_Mesh = new Mesh(vertices, indices);

}

Terrain::Terrain(Terrain&& other) noexcept : textureRed(other.textureRed), textureGreen(other.textureBlue), 
	textureBlue(other.textureBlue), textureBackground(other.textureBackground), blendMap(other.blendMap),
	redNormal(other.redNormal), blueNormal(other.blueNormal),
	greenNormal(other.greenNormal), backgroundNormal(other.backgroundNormal) {

	this->m_Heights = other.m_Heights;
	this->m_Mesh = other.m_Mesh;

	other.m_Mesh = nullptr;

}

Terrain::~Terrain() {

	delete m_Mesh;

}

void Terrain::generateTerrain(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices,
	uint32_t xSize, uint32_t zSize, const char* heightMap, int height) {

	bool defaultHeightMap = false;
	int length, width, bpp;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* buffer = stbi_load(heightMap, &length, &width, &bpp, 1);

	if (!buffer) {
		buffer = new uint8_t[128 * 128];
		memset(buffer, 0, 128 * 128);
		length = width = 128;
		defaultHeightMap = true;
	}

	std::vector<Vertex3D> verticesAux;
	std::vector<uint32_t> indicesAux;
	uint32_t count = length * width;

	verticesAux.resize(count);
	indicesAux.resize(6 * (length - 1) * (width - 1));
	m_Heights.resize(count);

	int it = 0;
	for (int z = 0; z < width; z++) {
		for (int x = 0; x < length; x++) {
			verticesAux[it].coords.x = (float)x / ((float)(length - 1)) * xSize;
			verticesAux[it].coords.y = getHeight(x, z, length, width, height, buffer);
			m_Heights[it] = verticesAux[it].coords.y;
			verticesAux[it].coords.z = (float)z / ((float)(width - 1)) * zSize;

			verticesAux[it].normals = getNormals(x, z, length, width, height, buffer);

			verticesAux[it].uv.x = (float)x / ((float)(length - 1));
			verticesAux[it].uv.y = (float)z / ((float)(width - 1));

			it++;
		}
	}

	// Tangentes y Bitangentes
	for (int z = 0; z < width - 1; z++) {
		for (int x = 0; x < length - 1; x++) {
			
			int index0 = z * width + x;
			int index1 = (z + 1) * width + x;
			int index2 = z * width + (x + 1);

			auto deltaPos1 = verticesAux[index1].coords - verticesAux[index0].coords;
			auto deltaPos2 = verticesAux[index2].coords - verticesAux[index0].coords;

			auto deltaUV1 = verticesAux[index1].uv - verticesAux[index0].uv;
			auto deltaUV2 = verticesAux[index2].uv - verticesAux[index0].uv;

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

			auto tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			auto bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

			verticesAux[index0].tangents = tangent;
			verticesAux[index1].tangents = tangent;
			verticesAux[index2].tangents = tangent;

			verticesAux[index0].bitangents = bitangent;
			verticesAux[index1].bitangents = bitangent;
			verticesAux[index2].bitangents = bitangent;

		}
	}

	int indice = 0;
	for (int z = 0; z < width - 1; z++) {
		for (int x = 0; x < length - 1; x++) {
			indicesAux[indice++] = z * width + x; // Top Left
			indicesAux[indice++] = (z + 1) * width + x; // Bottom Left
			indicesAux[indice++] = z * width + x + 1; // Top Right
			indicesAux[indice++] = z * width + x + 1; // Top Right
			indicesAux[indice++] = (z + 1) * width + x; // Bottom Left
			indicesAux[indice++] = (z + 1) * width + x + 1; // Bottom Right
		}
	}

	if (buffer && !defaultHeightMap)
		stbi_image_free(buffer);
	else if (buffer && defaultHeightMap)
		delete[] buffer;

	vertices = verticesAux;
	indices = indicesAux;

}

float Terrain::getHeight(int x, int z, int length, int width, int height, uint8_t* data) {

	if (x < 0 || x > length || z < 0 || z > width) {
		return 0;
	}

	float result = data[x + z * width];
	result /= 255;
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
	shader->sendMat4("u_Model", glm::mat4(1.0f));
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

	float gridQuadSize = 20.0f / ((float)glm::sqrt(m_Heights.size()) - 1); // <- Hardcoding value !!!
	int gridX = ((int)(x / gridQuadSize)) + 1;
	int gridZ = ((int)(z / gridQuadSize)) + 1;

	if (gridX < 0 || gridZ < 0 || gridX > glm::sqrt(m_Heights.size()) - 1 || gridZ > glm::sqrt(m_Heights.size()) - 1)
		return 0;

	float xCoord = (x / gridQuadSize + 1) - gridX;
	float zCoord = (z / gridQuadSize + 1) - gridZ;

	float height;
	if (xCoord <= (1 - zCoord)) {
		height = baryCentric(glm::vec3(0, m_Heights[gridX + gridZ * 128], 0),
			glm::vec3(1, m_Heights[(gridX + 1) + gridZ * 128], 0),
			glm::vec3(0, m_Heights[gridX + (gridZ + 1) * 128], 1),
			glm::vec2(xCoord, zCoord));    // <- Hardcoding value
	}
	else {
		height = baryCentric(glm::vec3(1, m_Heights[(gridX + 1) + gridZ * 128], 0),
			glm::vec3(1, m_Heights[(gridX + 1) + (gridZ + 1) * 128], 1),
			glm::vec3(0, m_Heights[gridX + (gridZ + 1) * 128], 1),
			glm::vec2(xCoord, zCoord));    // <- Hardcoding value
	}

	return height;

}

float Terrain::baryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {

	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;

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

TerrainBuilder& TerrainBuilder::TilingFactor(float tilingFactor) {

	this->tilingFactor = tilingFactor;
	return *this;

}

Terrain TerrainBuilder::Build() {

	return Terrain(redDiffuseTex, greenDiffuseTex, blueDiffuseTex, diffuseTex, blendMap, heightMap, redNormalTex, greenNormalTex,
		blueNormalTex, normalTex, tilingFactor);

}

Terrain* TerrainBuilder::BuildHeap() {

	return new Terrain(redDiffuseTex, greenDiffuseTex, blueDiffuseTex, diffuseTex, blendMap, heightMap, redNormalTex, greenNormalTex,
		blueNormalTex, normalTex, tilingFactor);

}