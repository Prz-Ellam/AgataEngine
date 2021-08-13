#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <tuple>
#include <fstream>
#include <vector>
#include <sstream>
#include "BufferLayout.h"
#include "VertexArray.h"

class Loader {
public:
	Loader() = delete;
	~Loader() = delete;
	static bool loadOBJ(const std::string& filePath, std::vector<Vertex3D>& vertices, std::vector<unsigned int>& indices);

	static bool loadCube(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices);

	static std::tuple<std::vector<Vertex3D>, std::vector<uint32_t>> loadSkyBox(float size);
	static std::tuple<std::vector<Vertex3D>, std::vector<uint32_t>> loadQuad();
	static std::tuple<std::vector<Vertex3D>, std::vector<uint32_t>> loadHorizontalQuad();
	// loadTerrain
	// loadSkybox
	// loadSkydome
	// loadBillboard
	// loadWater

};

#endif