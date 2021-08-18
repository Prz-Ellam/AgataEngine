#include "Loader.h"
#include "Log.h"
#include <Windows.h>
#include "Timer.h"
#include "Animation.h"
#include "KeyFrames.h"
#include "Animation.h"
#include <utility>
#undef min
#undef max // <- Windows.h tiene definido esto


#define JOINT_MAX 3

bool Loader::loadOBJ(const std::string& filePath, std::vector<Vertex3D>& vertices, std::vector<unsigned int>& indices) {

	std::ifstream objReader(filePath);

	if (!objReader.is_open()) {
		return false;
	}

	std::vector<glm::vec3> tempVertPos;
	std::vector<glm::vec2> tempVertUV;
	std::vector<glm::vec3> tempVertNorm;

	std::vector<uint32_t> vertPosIndices;
	std::vector<uint32_t> uvIndices;
	std::vector<uint32_t> normIndices;

	std::vector<Vertex3D> verticesAux;

	std::string data;
	std::string prefix;
	std::stringstream line;
	while (!objReader.eof()) {

		std::getline(objReader, data);
		line.clear();
		line.str(data);
		line >> prefix;

		if (prefix == "v") {
			glm::vec3 v;
			line >> v.x >> v.y >> v.z;
			tempVertPos.push_back(v);
		}
		else if (prefix == "vn") {
			glm::vec3 vn;
			line >> vn.x >> vn.y >> vn.z;
			tempVertNorm.push_back(vn);
		}
		else if (prefix == "vt") {
			glm::vec2 vt;
			line >> vt.x >> vt.y;
			tempVertUV.push_back(vt);
		}
		else if (prefix == "f") {

			uint32_t counter = 0;
			uint32_t temp;

			while (line >> temp) {

				if (counter == 0) {
					vertPosIndices.push_back(temp);
				}
				else if (counter == 1) {
					uvIndices.push_back(temp);
				}
				else if (counter == 2) {
					normIndices.push_back(temp);
				}

				if (line.peek() == '/') {
					counter++;
					line.ignore(1, '/');
				}
				else if (line.peek() == ' ') {
					counter++;
					line.ignore(1, ' ');
				}

				if (counter > 2) {
					counter = 0;
				}

			}
		}
	}

	verticesAux.resize(vertPosIndices.size());
	indices.resize(vertPosIndices.size());
	for (uint32_t i = 0; i < vertPosIndices.size(); i++) {

		verticesAux[i].coords.x = (tempVertPos[vertPosIndices[i] - 1].x) / 39.3701;
		verticesAux[i].coords.y = (tempVertPos[vertPosIndices[i] - 1].y) / 39.3701;
		verticesAux[i].coords.z = (tempVertPos[vertPosIndices[i] - 1].z) / 39.3701;

		verticesAux[i].uv.x = tempVertUV[uvIndices[i] - 1].x;
		verticesAux[i].uv.y = tempVertUV[uvIndices[i] - 1].y;

		verticesAux[i].normals.x = (tempVertNorm[normIndices[i] - 1].x);
		verticesAux[i].normals.y = (tempVertNorm[normIndices[i] - 1].y);
		verticesAux[i].normals.z = (tempVertNorm[normIndices[i] - 1].z);

		indices[i] = i;

	}

	for (uint32_t i = 0; i < verticesAux.size(); i += 3) {

		glm::vec3& v0 = verticesAux[i + 0].coords;
		glm::vec3& v1 = verticesAux[i + 1].coords;
		glm::vec3& v2 = verticesAux[i + 2].coords;

		glm::vec2& uv0 = verticesAux[i + 0].uv;
		glm::vec2& uv1 = verticesAux[i + 1].uv;
		glm::vec2& uv2 = verticesAux[i + 2].uv;

		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;

		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);

		glm::vec3 tangent = r * (deltaUV2.y * deltaPos1 - deltaUV1.y * deltaPos2);
		glm::vec3 bitangent = r * (deltaUV1.x * deltaPos2 - deltaUV2.x * deltaPos1);

		verticesAux[i + 0].tangents = tangent;
		verticesAux[i + 1].tangents = tangent;
		verticesAux[i + 2].tangents = tangent;

		verticesAux[i + 0].bitangents = bitangent;
		verticesAux[i + 1].bitangents = bitangent;
		verticesAux[i + 2].bitangents = bitangent;

	}

	vertices = verticesAux;
	objReader.close();
	return true;

}

bool Loader::loadCollada(const std::string& filePath, std::vector<AnimVertex>& vertices, std::vector<uint32_t>& indices, 
	Joint& joints, std::vector<Animation>& animations, glm::mat4& globalInverse) {

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene) 
		return false;

	aiMesh* mesh = scene->mMeshes[0];

	globalInverse = glm::transpose(*reinterpret_cast<glm::mat4*>(&scene->mRootNode->mTransformation.Inverse()));

	std::vector<AnimVertex> verticesAux;
	for (int i = 0; i < scene->mNumMeshes; i++) {

		verticesAux.resize(mesh->mNumVertices);
		indices.resize(mesh->mNumVertices); // <- Clean up first

		for (int i = 0; i < mesh->mNumVertices; i++) {

			glm::vec3 v;
			v.x = mesh->mVertices[i].x;
			v.y = mesh->mVertices[i].y;
			v.z = mesh->mVertices[i].z;
			verticesAux[i].coords = v;;

			glm::vec2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			verticesAux[i].uv = uv;

			glm::vec3 norm;
			norm.x = mesh->mNormals[i].x;
			norm.y = mesh->mNormals[i].y;
			norm.z = mesh->mNormals[i].z;
			verticesAux[i].normals = norm;

			glm::vec3 tan;
			tan.x = mesh->mTangents[i].x;
			tan.y = mesh->mTangents[i].y;
			tan.z = mesh->mTangents[i].z;
			verticesAux[i].tangents = tan;
			
			glm::vec3 bi;
			bi.x = mesh->mBitangents[i].x;
			bi.y = mesh->mBitangents[i].y;
			bi.z = mesh->mBitangents[i].z;
			verticesAux[i].bitangents = bi;

			indices[i] = i;

		}
	}

	uint32_t jointID = 0;
	/* 
		jointPos acumula el offset en el que va los arreglos de joints y weights en cada uno de los vertices, todos inician en 0
	    y cuando se les introduce un joint aumentan uno, el maximo de joints son 3, en caso de haber mas de 3 seran ignorados
	*/
	std::vector<uint8_t> jointPos;
	jointPos.resize(verticesAux.size());

	std::unordered_map<uint32_t, std::pair<std::string, glm::mat4>> jointMap;
	

	for (int i = 0; i < mesh->mNumBones; i++) {

		/*	
			glm::mat4 y aiMatrix4x4 pesan 64 Bytes en memoria y ambas almacenan 16 float de 4 Bytes c/u asi que es seguro realizar
			esta conversion 
		*/
		glm::mat4 localTransform = *reinterpret_cast<glm::mat4*>(&mesh->mBones[i]->mOffsetMatrix);
		jointMap[jointID] = std::make_pair(mesh->mBones[i]->mName.C_Str(), localTransform);

		for (int j = 0; j < mesh->mBones[i]->mNumWeights; j++) {

			uint32_t vertexID = mesh->mBones[i]->mWeights[j].mVertexId; 
			float weight = mesh->mBones[i]->mWeights[j].mWeight;

			if (jointPos[vertexID] < JOINT_MAX) {
				verticesAux[vertexID].joints[jointPos[vertexID]] = jointID;
				verticesAux[vertexID].weights[jointPos[vertexID]] = weight;
				jointPos[vertexID]++;
			}

		}

		jointID++;

	}

	Joint jointsAux;
	findJointHierarchy(jointsAux, jointMap, scene->mRootNode);

	const aiAnimation* animator = scene->mAnimations[0];
	std::vector<const aiNodeAnim*> anims;

	for (int i = 0; i < animator->mNumChannels; i++) {
		anims.push_back(animator->mChannels[i]);
	}

	std::vector<Animation> jointAnimations;
	jointAnimations.resize(animator->mNumChannels);
	
	for (int i = 0; i < animator->mNumChannels; i++) {
		jointAnimations[i].setLength(scene->mAnimations[0]->mDuration);
		for (int j = 0; j < anims[0]->mNumPositionKeys; j++) {
			glm::vec3 pos = *reinterpret_cast<glm::vec3*>(&anims[i]->mPositionKeys[j].mValue);
			glm::quat rot = *reinterpret_cast<glm::quat*>(&anims[i]->mRotationKeys[j].mValue);
			glm::vec3 sca = *reinterpret_cast<glm::vec3*>(&anims[i]->mScalingKeys[j].mValue);
			KeyFrame keyframe = KeyFrame(anims[i]->mPositionKeys[j].mTime, pos, rot, sca);
			jointAnimations[i].addKeyFrame(keyframe);
		}
	}

	vertices = verticesAux;
	joints = jointsAux;
	animations = jointAnimations;
	return true;

}

bool Loader::loadCube(std::vector<Vertex3D>& vertices, std::vector<uint32_t>& indices) {

	std::vector<Vertex3D> verticesAux;

	verticesAux.resize(36);

	verticesAux[0].coords = glm::vec3(-0.5f, -0.5f, -0.5f);
	verticesAux[1].coords = glm::vec3(0.5f, -0.5f, -0.5f);
	verticesAux[2].coords = glm::vec3(0.5f, 0.5f, -0.5f);
	verticesAux[3].coords = glm::vec3(0.5f, 0.5f, -0.5f);
	verticesAux[4].coords = glm::vec3(-0.5f, 0.5f, -0.5f);
	verticesAux[5].coords = glm::vec3(-0.5f, -0.5f, -0.5f);

	verticesAux[6].coords = glm::vec3(-0.5f, -0.5f, 0.5f);
	verticesAux[7].coords = glm::vec3(0.5f, -0.5f, 0.5f);
	verticesAux[8].coords = glm::vec3(0.5f, 0.5f, 0.5f);
	verticesAux[9].coords = glm::vec3(0.5f, 0.5f, 0.5f);
	verticesAux[10].coords = glm::vec3(-0.5f, 0.5f, 0.5f);
	verticesAux[11].coords = glm::vec3(-0.5f, -0.5f, 0.5f);

	verticesAux[12].coords = glm::vec3(-0.5f, 0.5f, 0.5f);
	verticesAux[13].coords = glm::vec3(-0.5f, 0.5f, -0.5f);
	verticesAux[14].coords = glm::vec3(-0.5f, -0.5f, -0.5f);
	verticesAux[15].coords = glm::vec3(-0.5f, -0.5f, -0.5f);
	verticesAux[16].coords = glm::vec3(-0.5f, -0.5f, 0.5f);
	verticesAux[17].coords = glm::vec3(-0.5f, 0.5f, 0.5f);

	verticesAux[18].coords = glm::vec3(0.5f, 0.5f, 0.5f);
	verticesAux[19].coords = glm::vec3(0.5f, 0.5f, -0.5f);
	verticesAux[20].coords = glm::vec3(0.5f, -0.5f, -0.5f);
	verticesAux[21].coords = glm::vec3(0.5f, -0.5f, -0.5f);
	verticesAux[22].coords = glm::vec3(0.5f, -0.5f, 0.5f);
	verticesAux[23].coords = glm::vec3(0.5f, 0.5f, 0.5f);

	verticesAux[24].coords = glm::vec3(-0.5f, -0.5f, -0.5f);
	verticesAux[25].coords = glm::vec3(0.5f, -0.5f, -0.5f);
	verticesAux[26].coords = glm::vec3(0.5f, -0.5f, 0.5f);
	verticesAux[27].coords = glm::vec3(0.5f, -0.5f, 0.5f);
	verticesAux[28].coords = glm::vec3(-0.5f, -0.5f, 0.5f);
	verticesAux[29].coords = glm::vec3(-0.5f, -0.5f, -0.5f);

	verticesAux[30].coords = glm::vec3(-0.5f, 0.5f, -0.5f);
	verticesAux[31].coords = glm::vec3(0.5f, 0.5f, -0.5f);
	verticesAux[32].coords = glm::vec3(0.5f, 0.5f, 0.5f);
	verticesAux[33].coords = glm::vec3(0.5f, 0.5f, 0.5f);
	verticesAux[34].coords = glm::vec3(-0.5f, 0.5f, 0.5f);
	verticesAux[35].coords = glm::vec3(-0.5f, 0.5f, -0.5f);

	verticesAux[0].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[1].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[2].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[3].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[4].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[5].uv = glm::vec2(0.0f, 0.0f);

	verticesAux[6].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[7].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[8].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[9].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[10].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[11].uv = glm::vec2(0.0f, 0.0f);

	verticesAux[12].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[13].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[14].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[15].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[16].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[17].uv = glm::vec2(1.0f, 0.0f);

	verticesAux[18].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[19].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[20].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[21].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[22].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[23].uv = glm::vec2(1.0f, 0.0f);

	verticesAux[24].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[25].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[26].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[27].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[28].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[29].uv = glm::vec2(0.0f, 1.0f);

	verticesAux[30].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[31].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[32].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[33].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[34].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[35].uv = glm::vec2(0.0f, 1.0f);

	std::vector<uint32_t> indexCube = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
											17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35 };


	vertices = verticesAux;
	indices = indexCube;

	return true;
}

std::tuple<std::vector<glm::vec3>, std::vector<uint32_t>> Loader::loadSkyBox(float size) {

	std::vector<glm::vec3> verticesAux(8);
	verticesAux[0] = glm::vec3(-size, -size, size);
	verticesAux[1] = glm::vec3(size, -size, size);
	verticesAux[2] = glm::vec3(size, -size, -size);
	verticesAux[3] = glm::vec3(-size, -size, -size);
	verticesAux[4] = glm::vec3(-size, size, size);
	verticesAux[5] = glm::vec3(size, size, size);
	verticesAux[6] = glm::vec3(size, size, -size);
	verticesAux[7] = glm::vec3(-size, size, -size);

	std::vector<uint32_t> indicesAux(36);
	indicesAux = { 1, 2, 6,
					6, 5, 1,
					0, 4, 7,
					7, 3, 0,
					4, 5, 6,
					6, 7, 4,
					0, 3, 2,
					2, 1, 0,
					0, 1, 5,
					5, 4, 0,
					3, 7, 6,
					6, 2, 3 };

	return { verticesAux, indicesAux };

}

std::tuple<std::vector<Vertex3D>, std::vector<uint32_t>> Loader::loadQuad() {

	std::vector<Vertex3D> verticesAux;
	verticesAux.resize(4);
	verticesAux[0].coords = glm::vec3(-1.0f, -1.0f, 0.0f);
	verticesAux[0].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[0].normals = glm::vec3(0.0f, 0.0f, -1.0f);
	verticesAux[1].coords = glm::vec3(1.0f, -1.0f, 0.0f);
	verticesAux[1].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[1].normals = glm::vec3(0.0f, 0.0f, -1.0f);
	verticesAux[2].coords = glm::vec3(1.0f, 1.0f, 0.0f);
	verticesAux[2].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[2].normals = glm::vec3(0.0f, 0.0f, -1.0f);
	verticesAux[3].coords = glm::vec3(-1.0f, 1.0f, 0.0f);
	verticesAux[3].uv = glm::vec2(0.0f, 1.0f);
	verticesAux[3].normals = glm::vec3(0.0f, 0.0f, -1.0f);

	std::vector<uint32_t> indicesAux;
	indicesAux.resize(6);
	indicesAux[0] = 0;
	indicesAux[1] = 1;
	indicesAux[2] = 2;
	indicesAux[3] = 2;
	indicesAux[4] = 3;
	indicesAux[5] = 0;

	return { verticesAux, indicesAux };

}

std::tuple<std::vector<Vertex3D>, std::vector<uint32_t>> Loader::loadHorizontalQuad() {

	std::vector<Vertex3D> verticesAux;
	verticesAux.resize(4);
	verticesAux[0].coords = glm::vec3(-0.5f, 0.0f, -0.5f);
	verticesAux[0].uv = glm::vec2(0.0f, 0.0f);
	verticesAux[1].coords = glm::vec3(0.5f, 0.0f, -0.5f);
	verticesAux[1].uv = glm::vec2(1.0f, 0.0f);
	verticesAux[2].coords = glm::vec3(0.5f, 0.0f, 0.5f);
	verticesAux[2].uv = glm::vec2(1.0f, 1.0f);
	verticesAux[3].coords = glm::vec3(-0.5f, 0.0f, 0.5f);
	verticesAux[3].uv = glm::vec2(0.0f, 1.0f);

	std::vector<uint32_t> indicesAux;
	indicesAux.resize(6);
	indicesAux[0] = 0;
	indicesAux[1] = 1;
	indicesAux[2] = 2;
	indicesAux[3] = 2;
	indicesAux[4] = 3;
	indicesAux[5] = 0;

	return { verticesAux, indicesAux };

}


void Loader::findJointHierarchy(Joint& joint, std::unordered_map<uint32_t, std::pair<std::string, glm::mat4>> jointMap,
	aiNode* node) {

	auto it = std::find_if(std::begin(jointMap), std::end(jointMap), [&node](auto&& p) { return p.second.first == node->mName.C_Str(); });

	if (it != jointMap.end()) {
		glm::mat4 transformationMatrix = *reinterpret_cast<glm::mat4*>(&node->mTransformation);
		joint.setAttributes(it->first, it->second.first, it->second.second, transformationMatrix);
		joint.addChildren(node->mNumChildren);
		
		for (int i = 0; i < node->mNumChildren; i++) {
			findJointHierarchy(joint.getChildren(i), jointMap, node->mChildren[i]);
		}
	}
	else {
		for (int i = 0; i < node->mNumChildren; i++) {
			findJointHierarchy(joint, jointMap, node->mChildren[i]);
		}
	}

}