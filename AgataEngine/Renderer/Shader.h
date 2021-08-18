#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
	~Shader();
	void bind();
	void unbind();
	void sendFloat(const std::string& uniform, float value);
	void sendVec3(const std::string& uniform, const glm::vec3& value);
	void sendVec4(const std::string& uniform, const glm::vec4& value);
	void sendInt1(const std::string& uniform, int value);
	void sendMat4(const std::string& uniform, const glm::mat4& value);

	void sendMat4Array(const std::string& uniform, const std::vector<glm::mat4>& value);


private:
	uint32_t m_ID;
	std::unordered_map<std::string, int> m_UniformLocationsCache;
	std::string readShaderFile(const std::string& filePath);
	unsigned int createShader(uint32_t type, const std::string& filePath);
	int getUniformLocation(const std::string&);
};

#endif