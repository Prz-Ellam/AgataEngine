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
#include "Fire.h"
#include "GrassManager.h"
#include <memory>

namespace Agata {

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
		Fire* fire;
		Zoom* spyGlassZoom;
		std::shared_ptr<Shader> shaderModel;
		std::shared_ptr<Shader> shaderAnimatedModel;
		std::shared_ptr<Shader> shaderTerrain;
		std::shared_ptr<Shader> shaderSkybox;
		std::shared_ptr<Shader> shaderBill;
		std::shared_ptr<Shader> shaderGrass;
		std::shared_ptr<Shader> shaderFire;
		std::shared_ptr<Shader> shaderWater;
		std::shared_ptr<Shader> shaderZoom;
		//std::shared_ptr<Shader> shaderFire;
		FrameBuffer* fbo;
		Camera* m_Camera;
		Camera* m_InvertCamera;
		Water* water;
		Light* light;
		GrassManager* grass;
		float lightAngle;
	};

}


#endif