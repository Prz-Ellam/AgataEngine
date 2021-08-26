#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>
#include "Terrain.h"
#include "Events/MouseMoveEvent.h"

class PerspectiveCameraProps {
public:
	PerspectiveCameraProps(float fov, float aspect, float nearPlane, float farPlane);
	float aspect;
	float nearPlane, farPlane;
	float fov;
};

class Camera {
private:
	PerspectiveCameraProps m_Properties;
	glm::vec3 m_Position;
	glm::vec3 m_ThirdPosition;
	glm::vec3 m_Forward, m_Right, m_Up;
	float m_Pitch, m_Yaw;
	float m_Speed, m_Sensitivity;
	glm::mat4 m_View, m_Projection;
	int m_PitchDirection = 1;

	float m_YawVelocity, m_YawAcceleration;
	float m_PitchVelocity, m_PitchAcceleration;

	float m_ForwardVelocity, m_ForwardAcceleration;
	float m_RightVelocity, m_RightAcceleration;

	float m_UpVelocity, m_UpAcceleration;

	bool m_Fly;

	float lastX, lastY;
	bool firstMouse;


	// tempora
	bool isPressed = false;

public:
	Camera(const PerspectiveCameraProps& properties, float speed, float sensitivity);
	void update();
	void move(GLFWwindow* window, Terrain& terrain, float deltaTime);
	glm::vec3& getPosition();

	void move(GLFWwindow* window, MouseMoveEvent e);

	void setSpeed(float speed);
	void setSensitivity(float sensitivity);

	void setPosition(const glm::vec3& position) { m_Position = position; }
	glm::vec3& getRight() { return m_Right; }
	glm::vec3& getForward() { return m_Forward; }
	glm::vec3& getUp() { return m_Up; }
	float& getUpVelocityRef() { return m_UpVelocity; }
	float& getPitch() { return m_Pitch; }
	float& getYaw() { return m_Yaw; }
	glm::mat4 getView() const;
	glm::mat4 getProjection() const;
	void setView(float fov, float aspect, float nearPlane, float farPlane);

	PerspectiveCameraProps& getPerspectiveCameraPropsRef() { return m_Properties; }

	void moveHeight(float height);

	void changePitchDirection();

	// bool isFly();
};

#endif