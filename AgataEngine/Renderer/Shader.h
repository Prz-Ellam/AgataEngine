#ifndef AGATA_SHADER_H
#define AGATA_SHADER_H

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Agata {

	class Shader {
	public:
		Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath);
		Shader(const Shader& other) = delete;
		Shader(Shader&& other) noexcept;
		Shader& operator=(const Shader& other) = delete;
		~Shader();

		void bind() const;
		void unbind() const;

		void sendFloat(const std::string& uniform, float value);
		void sendInt1(const std::string& uniform, int value);
		void sendIntArray(const std::string& uniform, const int* data, size_t count);
		void sendVec3(const std::string& uniform, const glm::vec3& value);
		void sendVec4(const std::string& uniform, const glm::vec4& value);
		void sendMat4(const std::string& uniform, const glm::mat4& value);
	private:
		uint32_t m_ID;
		std::unordered_map<std::string, int> m_UniformLocationsCache;
		std::string readShaderFile(const std::string& filePath);
		uint32_t createShader(uint32_t type, const std::string& filePath);
		int getUniformLocation(const std::string& uniform);
	};

}

#endif