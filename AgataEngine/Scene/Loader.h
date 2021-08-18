#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <tuple>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <unordered_map>
#include "BufferLayout.h"
#include "VertexArray.h"
#include "Joint.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Animation.h"

class Loader {
public:
	Loader() = delete;
	~Loader() = delete;
	static bool loadOBJ(const std::string& filePath, std::vector<Vertex3D>& vertices, std::vector<unsigned int>& indices);

	static bool loadCollada(const std::string& filePath, std::vector<AnimVertex>& vertices, std::vector<uint32_t>& indices,
	Joint& joints, std::vector<Animation>& animations, glm::mat4& globalInverse);

	static void findJointHierarchy(Joint& joint, std::unordered_map<uint32_t, std::pair<std::string, glm::mat4>> jointMap, 
		aiNode* node);

	static bool loadCube(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices);

	static std::tuple<std::vector<glm::vec3>, std::vector<uint32_t>> loadSkyBox(float size);
	static std::tuple<std::vector<Vertex3D>, std::vector<uint32_t>> loadQuad();
	static std::tuple<std::vector<Vertex3D>, std::vector<uint32_t>> loadHorizontalQuad();
	// loadTerrain
	// loadSkybox
	// loadSkydome
	// loadBillboard
	// loadWater

};

#endif