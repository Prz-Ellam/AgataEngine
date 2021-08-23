#include "AnimatedModel.h"
#include "Renderer.h"
#include "Loader.h"
#include "Log.h"
#include "glmUtils.h"

AnimatedModel::AnimatedModel(const std::string& filePath, const std::string& diffuseTex, const std::string& specularTex, 
	const std::string& normalTex, const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale,
	const glm::vec3& ambientMaterial, const glm::vec3& diffuseMaterial, const glm::vec3& specularMaterial, float shininessMaterial)
	: m_Position(position), m_Rotation(rotation), m_Scale(scale),
	m_Material(diffuseTex, specularTex, normalTex, ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial) {

	m_JointCount = 0;

	this->loadModel(filePath);

	//std::vector<AnimVertex> vertices;
	//std::vector<uint32_t> indices;
	//
	//Loader::loadCollada(filePath, vertices, indices, m_RootJoint, animations, m_GlobalInverseTransformation);
	//m_JointCount = animations.size();
	//
	//m_Mesh = new Mesh(vertices, indices);

	m_Transformation = transformationMatrix(m_Position, m_Rotation, m_Scale);

}

void AnimatedModel::setPosition(const glm::vec3& position) {

	m_Position = position;
	m_Transformation = transformationMatrix(m_Position, m_Rotation, m_Scale);

}

void AnimatedModel::setRotation(const glm::vec3& rotation) {

	m_Rotation = rotation;
	m_Transformation = transformationMatrix(m_Position, m_Rotation, m_Scale);

}

glm::mat4 AnimatedModel::getTransformation() const {

	return m_Transformation;

}

void AnimatedModel::draw(std::shared_ptr<Shader> shader, Light& light, float ts, const glm::vec4& clipDistance) {

	std::vector<glm::mat4> transformations;
	loadJointsTransforms(ts, transformations);

	shader->bind();

	for (int i = 0; i < m_JointCount; i++) {
		std::string name = "u_Joints[" + std::to_string(i) + "]";
		shader->sendMat4(name, transformations[i]);
	}

	shader->sendMat4("u_Model", m_Transformation);
	shader->sendMat4("u_View", Renderer::getView());
	shader->sendMat4("u_Projection", Renderer::getProjection());
	shader->sendVec3("u_CameraPos", Renderer::getPosition());
	shader->sendVec3("u_LightPos", light.getPosition());
	shader->sendVec3("u_LightColour", light.getColour());
	shader->sendInt1("u_DiffuseMap", m_Material.sendDiffuseTexture(0));
	shader->sendInt1("u_SpecularMap", m_Material.sendSpecularTexture(1));
	shader->sendInt1("u_NormalMap", m_Material.sendNormalTexture(2));
	shader->sendVec4("u_Plane", clipDistance);
	shader->sendVec4("u_Colour", m_Colour);
	shader->sendVec3("u_AmbientMaterial", m_Material.getAmbient());
	shader->sendVec3("u_DiffuseMaterial", m_Material.getDiffuse());
	shader->sendVec3("u_SpecularMaterial", m_Material.getSpecular());
	shader->sendFloat("u_ShininessMaterial", m_Material.getShininess());

	for (int i = 0; i < meshes.size(); i++) {
		Renderer::drawIndex(meshes[i].getVertexArray(), meshes[i].getIndexCount());
	}

}

void AnimatedModel::loadModel(const std::string& path) {

	scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_CalcTangentSpace);

	if (!scene || !scene->mRootNode || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE) {
		Log::error("Error Assimp: %s", importer.GetErrorString());
		assert(0);
		return;
	}

	m_GlobalInverseTransformation = assimpToGlmMatrix(scene->mRootNode->mTransformation);
	m_GlobalInverseTransformation = glm::inverse(m_GlobalInverseTransformation);

	if (scene->mAnimations[0]->mTicksPerSecond != 0.0) {
		ticksPerSec = scene->mAnimations[0]->mTicksPerSecond;
	}
	else {
		ticksPerSec = 25.0f;
	}

	processNode(scene->mRootNode, scene);

	/*
		Load the aiNodeAnim that contains all keyframes of a Joint
	*/
	const aiAnimation* animation = scene->mAnimations[0];
	for (int i = 0; i < animation->mNumChannels; i++) {
		const aiNodeAnim* nodeAnim = animation->mChannels[i];
		animationInfo[nodeAnim->mNodeName.C_Str()] = nodeAnim;
	}

}

void AnimatedModel::processNode(aiNode* node, const aiScene* scene) {

	for (uint32_t i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* ai_mesh = scene->mMeshes[i];
		std::vector<AnimVertex> vertices;
		std::vector<uint32_t>indices;
		processMesh(ai_mesh, scene, vertices, indices);
		meshes.push_back(Mesh(vertices, indices));
	}

}

Mesh AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene, std::vector<AnimVertex>& vertices, std::vector<uint32_t>& indices) {

	vertices = {};
	indices = {};
	vertices.resize(mesh->mNumVertices);
	indices.reserve((__int64)mesh->mNumFaces * 3);

	/*
		Load all vertex data
	*/
	for (uint32_t i = 0; i < mesh->mNumVertices; i++) {

		glm::vec3 v;
		v.x = mesh->mVertices[i].x;
		v.y = mesh->mVertices[i].y;
		v.z = mesh->mVertices[i].z;
		vertices[i].coords = v;;

		glm::vec2 uv;
		uv.x = mesh->mTextureCoords[0][i].x;
		uv.y = mesh->mTextureCoords[0][i].y;
		vertices[i].uv = uv;

		glm::vec3 norm;
		norm.x = mesh->mNormals[i].x;
		norm.y = mesh->mNormals[i].y;
		norm.z = mesh->mNormals[i].z;
		vertices[i].normals = norm;

		glm::vec3 tan;
		tan.x = mesh->mTangents[i].x;
		tan.y = mesh->mTangents[i].y;
		tan.z = mesh->mTangents[i].z;
		vertices[i].tangents = tan;

		glm::vec3 bi;
		bi.x = mesh->mBitangents[i].x;
		bi.y = mesh->mBitangents[i].y;
		bi.z = mesh->mBitangents[i].z;
		vertices[i].bitangents = bi;

	}

	/*
		Load indices
	*/
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace& face = mesh->mFaces[i]; 
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	/*
		Load Joints
	*/
	std::vector<uint8_t> jointPos(vertices.size());
	for (uint32_t i = 0; i < mesh->mNumBones; i++) {

		uint32_t jointIndex = 0;
		aiBone* joint = mesh->mBones[i];
		std::string jointName = joint->mName.C_Str();

		if (jointInfo.find(jointName) == jointInfo.end()) {

			jointIndex = m_JointCount;
			m_JointCount++;

			glm::mat4 offsetMatrix = assimpToGlmMatrix(joint->mOffsetMatrix);
			jointTransformations.push_back(std::make_pair(offsetMatrix, glm::mat4(1.0f)));
			jointTransformations[jointIndex].first = assimpToGlmMatrix(joint->mOffsetMatrix);

			jointInfo[jointName] = jointIndex;

		}
		else {
			jointIndex = jointInfo[jointName];
		}

		for (int j = 0; j < joint->mNumWeights; j++) {

			uint32_t vertexID = joint->mWeights[j].mVertexId;
			float weight = joint->mWeights[j].mWeight;

			if (jointPos[vertexID] < 3) {
				vertices[vertexID].joints[jointPos[vertexID]] = jointIndex;
				vertices[vertexID].weights[jointPos[vertexID]] = weight;
				jointPos[vertexID]++;
			}
		}
	}

	return Mesh(vertices, indices);

}

void AnimatedModel::loadJointsTransforms(float timeStamp, std::vector<glm::mat4>& transformations) {

	float timeTicks = timeStamp * ticksPerSec;
	float animationTime = fmod(timeTicks, scene->mAnimations[0]->mDuration);
	findTransformations(animationTime, scene->mRootNode);

	transformations.resize(m_JointCount);
	for (uint32_t i = 0; i < m_JointCount; i++) {
		transformations[i] = jointTransformations[i].second;
	}

}

void AnimatedModel::findTransformations(float timeStamp, const aiNode* node, const glm::mat4 parentTransformation) {

	std::string nodeName(node->mName.data);
	glm::mat4 nodeTransform = assimpToGlmMatrix(node->mTransformation);

	auto it = animationInfo.find(nodeName);
	const aiNodeAnim* nodeAnim;
	if (it != animationInfo.end()) {

		nodeAnim = it->second;
		glm::vec3 lerpPos = lerpPosition(timeStamp, nodeAnim);
		glm::quat lerpRot = lerpRotation(timeStamp, nodeAnim);
		glm::vec3 lerpSca = lerpScale(timeStamp, nodeAnim);

		nodeTransform = glm::translate(glm::mat4(1.0f), lerpPos);
		nodeTransform = nodeTransform * glm::mat4_cast(lerpRot);
		nodeTransform = glm::scale(nodeTransform, lerpSca);

	}

	glm::mat4 globalTransform = parentTransformation * nodeTransform;

	if (jointInfo.find(nodeName) != jointInfo.end()) {
		uint32_t jointID = jointInfo[nodeName];
		jointTransformations[jointID].second = m_GlobalInverseTransformation * globalTransform 
			* jointTransformations[jointID].first;
	}

	for (uint32_t i = 0; i < node->mNumChildren; i++) {
		findTransformations(timeStamp, node->mChildren[i], globalTransform);
	}

}

glm::vec3 AnimatedModel::lerpPosition(float animationTime, const aiNodeAnim* node) {

	if (node->mNumPositionKeys == 1) {
		return assimpToGlmVec3(node->mPositionKeys[0].mValue);
	}

	int positionIndex = 0, nextPosition;
	for (uint32_t i = 0; i < node->mNumPositionKeys - 1; i++)  {
		if (animationTime < (float)node->mPositionKeys[i + 1].mTime) {
			positionIndex = i;
			break;
		}
	}

	nextPosition = positionIndex + 1;
	float dt = (float)(node->mPositionKeys[nextPosition].mTime - node->mPositionKeys[positionIndex].mTime);
	float factor = (animationTime - (float)node->mPositionKeys[positionIndex].mTime) / dt;
	glm::vec3 start = assimpToGlmVec3(node->mPositionKeys[positionIndex].mValue);
	glm::vec3 end = assimpToGlmVec3(node->mPositionKeys[nextPosition].mValue);
	glm::vec3 delta = end - start;

	return start + factor * delta;

}

glm::quat AnimatedModel::lerpRotation(float animationTime, const aiNodeAnim* node) {

	if (node->mNumRotationKeys == 1) {
		return assimpToGlmQuat(node->mRotationKeys[0].mValue);
	}

	int positionIndex = 0, nextPosition;
	for (uint32_t i = 0; i < node->mNumRotationKeys - 1; i++) {
		if (animationTime < (float)node->mRotationKeys[i + 1].mTime) {
			positionIndex = i;
			break;
		}
	}

	nextPosition = positionIndex + 1;
	float dt = node->mRotationKeys[nextPosition].mTime - node->mRotationKeys[positionIndex].mTime;
	float factor = (animationTime - node->mRotationKeys[positionIndex].mTime) / dt;
	glm::quat start = assimpToGlmQuat(node->mRotationKeys[positionIndex].mValue);
	glm::quat end = assimpToGlmQuat(node->mRotationKeys[nextPosition].mValue);

	start = glm::normalize(start);
	end = glm::normalize(end);

	glm::quat result;
	float dot = start.x * end.x + start.y * end.y + start.z * end.z + start.w * end.w;
	float one_minus_blend = 1.0f - factor;

	if (dot < 0.0f)
	{
		result.x = start.x * one_minus_blend + factor * -end.x;
		result.y = start.y * one_minus_blend + factor * -end.y;
		result.z = start.z * one_minus_blend + factor * -end.z;
		result.w = start.w * one_minus_blend + factor * -end.w;
	}
	else
	{
		result.x = start.x * one_minus_blend + factor * end.x;
		result.y = start.y * one_minus_blend + factor * end.y;
		result.z = start.z * one_minus_blend + factor * end.z;
		result.w = start.w * one_minus_blend + factor * end.w;
	}

	return glm::normalize(result);

}

glm::vec3 AnimatedModel::lerpScale(float animationTime, const aiNodeAnim* node) {

	if (node->mNumScalingKeys == 1) {
		return assimpToGlmVec3(node->mScalingKeys[0].mValue);
	}

	int positionIndex = 0, nextPosition;
	for (uint32_t i = 0; i < node->mNumScalingKeys - 1; i++) {
		if (animationTime < (float)node->mScalingKeys[i + 1].mTime) {
			positionIndex = i;
			break;
		}
	}

	nextPosition = positionIndex + 1;
	float dt = (float)(node->mScalingKeys[nextPosition].mTime - node->mScalingKeys[positionIndex].mTime);
	float factor = (animationTime - (float)node->mScalingKeys[positionIndex].mTime) / dt;
	glm::vec3 start = assimpToGlmVec3(node->mScalingKeys[positionIndex].mValue);
	glm::vec3 end = assimpToGlmVec3(node->mScalingKeys[nextPosition].mValue);
	glm::vec3 delta = end - start;

	return start + factor * delta;

}







AnimatedModelBuilder& AnimatedModelBuilder::ModelPath(const std::string& filePath) {

	modelFilePath = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::DiffuseTexture(const std::string& filePath) {

	diffuseTexture = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::SpecularTexture(const std::string& filePath) {

	specularTexture = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::NormalTexture(const std::string& filePath) {

	normalTexture = filePath;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::Position(const glm::vec3& position) {

	this->position = position;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::Rotation(const glm::vec3& rotation) {

	this->rotation = rotation;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::Scale(const glm::vec3& scale) {

	this->scale = scale;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::AmbientMaterial(const glm::vec3& ambient) {

	this->ambientMaterial = ambient;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::DiffuseMaterial(const glm::vec3& diffuse) {

	this->diffuseMaterial = diffuse;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::SpecularMaterial(const glm::vec3& specular) {

	this->specularMaterial = specular;
	return *this;

}

AnimatedModelBuilder& AnimatedModelBuilder::ShininessMaterial(float shininess) {

	this->shininessMaterial = shininess;
	return *this;

}

AnimatedModel AnimatedModelBuilder::Build() {

	return AnimatedModel(modelFilePath, diffuseTexture, specularTexture, normalTexture, position, rotation, scale,
		ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial);

}

AnimatedModel* AnimatedModelBuilder::BuildHeap() {

	return new AnimatedModel(modelFilePath, diffuseTexture, specularTexture, normalTexture, position, rotation, scale,
		ambientMaterial, diffuseMaterial, specularMaterial, shininessMaterial);

}