#include "Camera.h"
#include "Log.h"

PerspectiveCameraProps::PerspectiveCameraProps(float fov, float aspect, float nearPlane, float farPlane) {

	this->fov = fov;
	this->aspect = aspect;
	this->nearPlane = nearPlane;
	this->farPlane = farPlane;

}

Camera::Camera(const PerspectiveCameraProps& properties, float speed, float sensitivity)
: m_Position(glm::vec3(0.0f, 0.0f, 0.0f)), m_Speed(speed), m_Sensitivity(sensitivity), m_Properties(properties) {

	m_Forward = glm::vec3(0.0f, 0.0f, 1.0f);
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_Right = glm::normalize(glm::cross(m_Forward, m_Up));

	m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	m_Projection = glm::perspective(glm::radians(m_Properties.fov), m_Properties.aspect, m_Properties.nearPlane, m_Properties.farPlane);

	m_Pitch = 0.0f;
	m_Yaw = 90.0f;

	m_Fly = false;

	m_YawVelocity = 0.0f;
	m_YawAcceleration = 0.0f;
	m_PitchVelocity = 0.0f;
	m_PitchAcceleration = 0.0f;
	m_ForwardVelocity = 0.0f;
	m_ForwardAcceleration = 0.0f;
	m_RightVelocity = 0.0f;
	m_RightAcceleration = 0.0f;
	m_UpVelocity = 0.0f;

}

void Camera::update() {

	//m_ThirdPosition = m_Position + m_Forward * 2.0f;
	//m_View = glm::lookAt(m_ThirdPosition, m_ThirdPosition + m_Forward * -1.0f, m_Up);
	m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);

	m_Projection = glm::perspective(glm::radians(m_Properties.fov), m_Properties.aspect, m_Properties.nearPlane, m_Properties.farPlane);

}

void Camera::move(GLFWwindow * window, Terrain &terrain, float deltaTime) {

	m_ForwardAcceleration = 0.0f;
	m_RightAcceleration = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		//m_Position += m_Speed * m_Forward * deltaTime * 50.0f;
		//m_Position += m_Speed * (m_Forward * -1.0f) * deltaTime;
		//m_SpeedForward = 0.1f;
		m_ForwardAcceleration += m_Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		//m_Position -= m_Speed * m_Forward * deltaTime * 50.0f;
		//m_Position -= m_Speed * (m_Forward * -1.0f) * deltaTime;
		//m_SpeedForward = -0.1f;
		m_ForwardAcceleration -= m_Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		//m_Position -= m_Speed * m_Right * deltaTime * 50.0f;
		//m_Position -= m_Speed * (m_Right * -1.0f) * deltaTime;
		//m_SpeedRight = 0.1f;
		m_RightAcceleration -= m_Speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		//m_Position += m_Speed * m_Right * deltaTime * 50.0f;
		//m_Position += m_Speed * (m_Right * -1.0f) * deltaTime;
		//m_SpeedRight = -0.1f;
		m_RightAcceleration += m_Speed;
	}

	m_ForwardAcceleration -= m_ForwardVelocity * 10.0f;
	m_Position += (m_ForwardVelocity * deltaTime + m_ForwardAcceleration * deltaTime * deltaTime * 0.5f) * m_Forward;
	m_ForwardVelocity = m_ForwardVelocity + m_ForwardAcceleration * deltaTime;

	m_RightAcceleration -= m_RightVelocity * 10.0f;
	m_Position += (m_RightVelocity * deltaTime + m_RightVelocity * deltaTime * deltaTime * 0.5f) * m_Right;
	m_RightVelocity = m_RightVelocity + m_RightAcceleration * deltaTime;

	m_YawAcceleration = 0.0f;
	m_PitchAcceleration = 0.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		m_YawAcceleration -= m_Sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		m_YawAcceleration += m_Sensitivity;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		m_PitchAcceleration += m_Sensitivity * m_PitchDirection;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		m_PitchAcceleration -= m_Sensitivity * m_PitchDirection;
	}

	m_PitchAcceleration -= m_PitchVelocity * 12.0f;
	m_Pitch = glm::clamp(m_Pitch + m_PitchVelocity * deltaTime + m_PitchAcceleration * deltaTime * deltaTime * 0.5f, -89.0f, 89.0f);
	m_PitchVelocity = m_PitchVelocity + m_PitchAcceleration * deltaTime;

	m_YawAcceleration -= m_YawVelocity * 12.0f;
	m_Yaw = m_Yaw + m_YawVelocity * deltaTime + m_YawAcceleration * deltaTime * deltaTime * 0.5f;
	m_YawVelocity = m_YawVelocity + m_YawAcceleration * deltaTime;

	glm::vec3 direction;
	direction.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
	direction.y = glm::sin(glm::radians(m_Pitch));
	direction.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

	m_Forward = glm::normalize(direction);
	m_Right = glm::normalize(glm::cross(m_Forward, m_Up));

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isPressed == false) {
		m_UpVelocity = 4.0f;
		m_Position += 0.001f;
		m_Fly = true;
		isPressed = true;
	}
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE) {
		isPressed = false;
	}

	if (m_Position.y > terrain.getHeight(m_Position.x, m_Position.z) + 0.6f && m_Fly == true) {
		m_Position.y = m_Position.y + m_UpVelocity * deltaTime + 9.81f * deltaTime * deltaTime * 0.5f;
		m_UpVelocity = -9.81f * deltaTime + m_UpVelocity;
	}
	else {
		m_Fly = false;
		m_Position.y = terrain.getHeight(m_Position.x, m_Position.z) + 0.6f;
		m_UpVelocity = 0.0f;
	}

	m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	m_Projection = glm::perspective(glm::radians(m_Properties.fov), m_Properties.aspect, m_Properties.nearPlane, m_Properties.farPlane);

}

void Camera::move(GLFWwindow* window, MouseMoveEvent e) {

	if (firstMouse) {
		lastX = e.getX();
		lastY = e.getY();
		firstMouse = false;
	}

	float xoffset = e.getX() - lastX;
	float yoffset = lastY - e.getY();
	lastX = e.getX();
	lastY = e.getY();

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	m_Yaw += xoffset;
	m_Pitch += yoffset * m_PitchDirection;

	if (m_Pitch > 89.0f)
		m_Pitch = 89.0f;
	if (m_Pitch < -89.0f)
		m_Pitch = -89.0f;

	glm::vec3 direction;
	direction.x = glm::cos(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));
	direction.y = glm::sin(glm::radians(m_Pitch));
	direction.z = glm::sin(glm::radians(m_Yaw)) * glm::cos(glm::radians(m_Pitch));

	m_Forward = glm::normalize(direction);
	m_Right = glm::normalize(glm::cross(m_Forward, m_Up));

	m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
	m_Projection = glm::perspective(glm::radians(m_Properties.fov), m_Properties.aspect, m_Properties.nearPlane, m_Properties.farPlane);

}

glm::vec3& Camera::getPosition() {

	return m_Position;

}

glm::mat4 Camera::getView() const {

	return m_View;

}

glm::mat4 Camera::getProjection() const {

	return m_Projection;

}

void Camera::moveHeight(float height) {

	m_Position.y += height;
	m_View = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);

}

void Camera::changePitchDirection() { 

	m_PitchDirection = -m_PitchDirection; 

}

void Camera::setView(float fov, float aspect, float nearPlane, float farPlane) {

	m_Properties.aspect = aspect;
	m_Properties.fov = fov;
	m_Properties.nearPlane = nearPlane;
	m_Properties.farPlane = farPlane;

}

void Camera::setSpeed(float speed) {

	m_Speed = speed;

}

void Camera::setSensitivity(float sensitivity) {

	m_Sensitivity = sensitivity;

}