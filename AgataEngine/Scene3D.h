#ifndef SCENE_3D_H
#define SCENE_3D_H

#include "Scene.h"
#include "Model.h"
#include "Terrain.h"
#include "Skybox.h"
#include "Shader.h"
#include "Billboard.h"
#include "AnimatedModel.h"
#include "Camera.h"
#include "Water.h"
#include "Light.h"
#include "ImGuiLayer.h"
#include "Zoom.h"
#include <memory>

class Scene3D : public Scene {
public:
	Scene3D();
	virtual ~Scene3D();

	void init() override;
	void shutdown() override;
	void setCallbacks() override;
	void run() override;

	void windowResizeEvent(WindowResizeEvent e) override;
	void windowCloseEvent(WindowCloseEvent e) override;
	void windowMoveEvent(WindowMoveEvent e) override;
	void mouseMoveEvent(MouseMoveEvent e) override;
	void keyEvent(KeyEvent e) override;
	void imGuiEvent(bool a) override;
private:
	ImGuiLayer* imGui;
	Model* model;
	AnimatedModel* animatedModel;
	Terrain* terrain;
	Skybox* skybox;
	Billboard* billboard;
	Zoom* spyGlassZoom;
	std::shared_ptr<Shader> shaderModel;
	std::shared_ptr<Shader> shaderTerrain;
	std::shared_ptr<Shader> shaderSkybox;
	std::shared_ptr<Shader> shaderBill;
	std::shared_ptr<Shader> shaderWater;
	std::shared_ptr<Shader> shaderZoom;
	//std::shared_ptr<Shader> shaderFire;
	Camera* m_Camera;
	Camera* m_InvertCamera;
	Water* water;
	Light* light;
	float lightAngle;
	glm::mat4 joint = glm::mat4(1.0f);
};


#endif