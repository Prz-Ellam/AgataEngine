#ifndef AGATA_LIGHT_H
#define AGATA_LIGHT_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Agata {

	class Light {
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Colour;
	public:
		Light(const glm::vec3& position, const glm::vec3& colour);
		glm::vec3& getPosition();
		glm::vec3& getColour();
		void setPosition(glm::vec3 position);
		void setColour(glm::vec3 colour);
		void setColourR(float r) { m_Colour.r += r; }
		void setColourG(float g) { m_Colour.g += g; }
		void setColourB(float b) { m_Colour.b += b; }
		void setPositionX(float x);
		void setPositionY(float y);
		void setPositionZ(float z);
		float getPositionX() { return m_Position.x; }
		float getPositionY() { return m_Position.y; }
		float getPositionZ() { return m_Position.z; }
	};

}

#endif