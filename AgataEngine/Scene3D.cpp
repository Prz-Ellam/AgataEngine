#include <GL/glew.h>
#include "Scene3D.h"
#include "Log.h"
#include "Loader.h"
#include "Timer.h"
#include <Windows.h>
#include "Input.h"
#include "Random.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <stb/stb_image_write.h>

namespace Agata {

	using namespace std::literals::string_literals;

	Scene3D::Scene3D() {

		init();

	}

	Scene3D::~Scene3D() {

		shutdown();

	}

	void Scene3D::init() {

		Agata::Random::init();

		m_Window = new Window(1280, 720, "Agata Engine"s);
		m_Window->setVSync(true);
		//m_Window->setIcons("images.jpg"s);

		m_Camera = new Camera(PerspectiveCameraProps(45.0f, (float)m_Window->getWidth() / m_Window->getHeight(), 0.1f, 1000.0f), 40.0f, 800.0f);
		m_InvertCamera = new Camera(PerspectiveCameraProps(45.0f, (float)m_Window->getWidth() / m_Window->getHeight(), 0.1f, 1000.0f), 40.0f, 800.0f);
		m_InvertCamera->changePitchDirection();

		m_Running = true;
		setCallbacks();

		m_Renderer.viewport(m_Window);

		if (glewInit() != GLEW_OK) {
			Log::error("ERROR: No se pudo iniciar GLEW\n");
			assert(0);
		}

		imGui = new ImGuiLayer(m_Window);
		imGui->attachDrawHandler(EVENT_FN(Scene3D::imGuiEvent));

		m_Renderer.enableBlend();
		glEnable(GL_DEPTH_TEST);

		m_Renderer.backCullface();
		lightAngle = 180.0f;

		{
			Agata::Timer modelBuilder("ModelBuilder");
			model = ModelBuilder::GenerateParams().
				ModelPath("Assets\\tea.obj"s).
				Position(glm::vec3(8.963f, 2.0f, 13.572f)).
				Rotation(glm::vec3(0.0f, 0.0f, 0.0f)).
				Scale(glm::vec3(2.0f)).
				DiffuseTexture("Assets\\diffuse.jpg"s).
				//SpecularTexture("Assets\\barrelSpecular.png"s).
				//NormalTexture("Assets\\normal.png"s).
				AmbientMaterial(glm::vec3(0.75f)).
				DiffuseMaterial(glm::vec3(0.85f)).
				SpecularMaterial(glm::vec3(1.0f)).
				ShininessMaterial(32).
				BuildHeap();

			animatedModel = AnimatedModelBuilder::GenerateParams().
				ModelPath("Assets\\Character.fbx"s).
				Position(glm::vec3(6.10f, 1.4f, 11.12f)).
				Rotation(glm::vec3(0.0f, 0.0f, 0.0f)).
				Scale(glm::vec3(0.01f)).
				DiffuseTexture("Assets\\CharacterDiffuse.jpg"s).
				//SpecularTexture("Assets\\barrelSpecular.png"s).
				//NormalTexture("Assets\\normal.png"s).
				AmbientMaterial(glm::vec3(0.75f)).
				DiffuseMaterial(glm::vec3(0.85f)).
				SpecularMaterial(glm::vec3(1.0f)).
				ShininessMaterial(32).
				BuildNew();
		}

		{
			Agata::Timer terrainBuilder("TerrainBuilder");
			terrain = TerrainBuilder::GenerateParams().
				RedTexture("Assets\\snow.jpg"s).
				GreenTexture("Multi1.jpg"s).
				BlueTexture("Assets\\dirt.jpg"s).
				BlackTexture("grass.jpg"s).
				BlendMap("Mini_BlendMap.jpg"s).
				HeightMap("Mini_HeightMap.jpg"s).
				RedNormal("Assets\\snowNorm.jpg"s).
				//GreenNormal(".png"s).
				BlueNormal("Assets\\dirtNorm.jpg"s).
				Position(glm::vec3(0.0f)).
				//BlackNormal(".png"s).
				Height(6.0f).
				Width(20.0f).
				Depth(20.0f).
				BuildNew();
		}
		texture = new Texture3D("01.bmp"s, "03.bmp"s, "05.bmp"s, "07.bmp"s, "02.bmp"s, "04.bmp"s);
		{
			Agata::Timer skyboxBuilder("SkyBoxBuilder");
			//skybox = new Skybox("Assets//Images//right.png", "Assets//Images//left.png", "Assets//Images//top.png",
			//	"Assets//Images//bottom.png", "Assets//Images//front.png", "Assets//Images//back.png", 500.0f);
			//skybox = new Skybox("Assets//Images//sky103ft.png", "Assets//Images//sky103bk.png", "Assets//Images//sky103up.png",
			//	"Assets//Images//sky103dn.png", "Assets//Images//sky103rt.png", "Assets//Images//sky103lf.png", 500.0f);
			skybox = new Skybox("Assets//Images//grimmnightft.png", "Assets//Images//grimmnightbk.png", "Assets//Images//grimmnightup.png",
				"Assets//Images//grimmnightdn.png", "Assets//Images//grimmnightrt.png", "Assets//Images//grimmnightlf.png", 500.0f);
		}
		//billboard = new Billboard("grass5.png"s, glm::vec3(4.87f, 1.735f, 8.7f), glm::vec3(0.4f));

		fire = new Fire("Assets\\rampFire.gif"s, "Assets\\alphaFire.gif"s, "Assets\\dudv.png"s, "Assets\\fireNoise.png"s, 
			glm::vec3(11.302f, 2.518f, 12.408f), glm::vec3(1.0f));

		spyGlassZoom = new Zoom("Assets\\zoom.jpg"s);

		{
			Agata::Timer shaders("Shaders");
			shaderModel = std::make_shared<Shader>("Assets//Shaders//ModelVertex.glsl", "Assets//Shaders//ModelFragment.glsl");
			shaderTerrain = std::make_shared<Shader>("Assets//Shaders//TerrainVertex.glsl", "Assets//Shaders//TerrainFragment.glsl");
			shaderSkybox = std::make_shared<Shader>("Assets//Shaders//SkyBoxVertex.glsl", "Assets//Shaders//SkyBoxFragment.glsl");
			shaderBill = std::make_shared<Shader>("Assets//Shaders//BillboardVertex.glsl", "Assets//Shaders//BillboardFragment.glsl");
			shaderFire = std::make_shared<Shader>("Assets//Shaders//FireVertex.glsl", "Assets//Shaders//FireFragment.glsl");
			shaderWater = std::make_shared<Shader>("Assets//Shaders//WaterVertex.glsl", "Assets//Shaders//WaterFragment.glsl");
			shaderZoom = std::make_shared<Shader>("Assets//Shaders//ZoomVertex.glsl", "Assets//Shaders//ZoomFragment.glsl");
			shaderGrass = std::make_shared<Shader>("Assets//Shaders//GrassVertex.glsl", "Assets//Shaders//GrassFragment.glsl");
			shaderAnimatedModel = std::make_shared<Shader>("Assets//Shaders//AnimatedModelVertex.glsl", "Assets//Shaders//ModelFragment.glsl");
			shaderCubeMap = std::make_shared<Shader>("Assets//Shaders//CubeMapVertex.glsl", "Assets//Shaders//CubeMapFragment.glsl");
		}

		{
			Agata::Timer wate("Water Loading");
			water = new Water(glm::vec3(5.0f, 1.11f, 4.658f), glm::vec3(4.0f, 1.0f, 5.0f), "dudv.png", "normalMap.png", 640, 480);
		}

		//light = new Light(glm::vec3(11.0f, 0.0f, -10.0f), glm::vec3(255.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f));
		//light = new Light(glm::vec3(11.0f, 14.5652, -3.7059), glm::vec3(1, 1, 1));
		 light = new Light(glm::vec3(11.0f, 14.5652, -3.7059), glm::vec3(20.0f / 255.0f, 20.0f / 255.0f, 20.0f / 255.0f));

		grass = new GrassManager(3);
		grass->addGrassUnit(glm::vec3(5.0f, 1.935f, 13.0f), glm::vec3(0.4f), 0);
		grass->addGrassUnit(glm::vec3(4.87f, 1.735f, 8.7f), glm::vec3(0.4f), 0);
		grass->addGrassUnit(glm::vec3(4.97f, 1.835f, 10.7f), glm::vec3(0.4f), 1);
		grass->addTexture("grass.png"s);
		grass->addTexture("grass5.png"s);

	}

	void Scene3D::shutdown() {

		delete imGui;
		delete m_Window;
		//delete model;
		delete grass;
		delete animatedModel;
		delete terrain;
		delete skybox;
		delete billboard;
		delete fire;
		delete water;
		delete light;
		delete m_Camera;
		delete m_InvertCamera;
		delete spyGlassZoom;

	}

	void Scene3D::setCallbacks() {

		m_Window->setWindowResizeEventHandler(EVENT_FN(Scene3D::windowResizeEvent));
		m_Window->setWindowCloseEventHandler(EVENT_FN(Scene3D::windowCloseEvent));
		m_Window->setWindowMoveEventHandler(EVENT_FN(Scene3D::windowMoveEvent));
		m_Window->setMouseMoveEventHandler(EVENT_FN(Scene3D::mouseMoveEvent));
		m_Window->setKeyEventHandler(EVENT_FN(Scene3D::keyEvent));
		m_Window->setJoyStickEventHandler(EVENT_FN(Scene3D::joyStickEvent));

	}

	void Scene3D::run() {

		float dt = 1 / 60;
		LARGE_INTEGER beginTime;
		QueryPerformanceCounter(&beginTime);

		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);
		double freq = (double)frequency.QuadPart;

		while (m_Running) {

			//m_Camera->setPosition(light->getPosition());

			//float r = glm::sqrt(1600 / (4 * pow(cos(glm::radians(lightAngle)), 2) + pow(sin(glm::radians(lightAngle)), 2)));
			//float x = r * cos(glm::radians(lightAngle));
			//float y = r * sin(glm::radians(lightAngle));
			//
			//light->setPositionY(y * 0.5f);
			//light->setPositionZ(x + 10);
			//
			//float intensity = glm::clamp(glm::sin(glm::radians(lightAngle)), 0.2f, 0.8f);
			//
			//if (lightAngle >= 0) {
			//	animatedModel->getMaterialRef().setDiffuse(glm::vec3(intensity + 0.2f));
			//	terrain->setDiffuse(glm::vec3(intensity + 0.2f));
			//}
			//else {
			//	animatedModel->getMaterialRef().setDiffuse(glm::vec3(0.0f));
			//	terrain->setDiffuse(glm::vec3(0.0f));
			//}
			//
			//animatedModel->getMaterialRef().setAmbient(glm::vec3(intensity));
			//terrain->setAmbient(glm::vec3(glm::clamp(intensity, 0.2f, 0.4f)));
			//
			////(i > -180.0f) ? i -= 0.1f : i = 180.0f;
			//(lightAngle > 0) ? lightAngle -= 0.1f : lightAngle = 180.0f;
			//
			//if (lightAngle < 160.0f && lightAngle > 120.0f) {
			//
			//	light->setColourG(0.00125f);
			//	light->setColourB(0.00125f);
			//
			//}
			//
			//if (lightAngle < 60.0f && lightAngle > 20.0f) {
			//
			//	light->setColourG(-0.00125f);
			//	light->setColourB(-0.00125f);
			//
			//}

			terrain->setAmbient(glm::vec3(0.4f));
			terrain->setDiffuse(glm::vec3(1.0f));

			if (imGuiActive) {
				imGui->newFrame();
				imGui->draw();
			}

			float ts = glfwGetTime();

			m_InvertCamera->move(m_Window->getHandler(), GLFW_JOYSTICK_1, *terrain, dt);
			m_Camera->move(m_Window->getHandler(), GLFW_JOYSTICK_1, *terrain, dt);

			glEnable(GL_CLIP_DISTANCE0);

			float distance = 2 * (m_Camera->getPosition().y - water->getHeight());
			m_InvertCamera->moveHeight(-distance);
			m_Renderer.beginScene(m_InvertCamera);
			water->startReflection();
			m_Renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);
			//animatedModel->draw(shaderModel, *light, ts, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			terrain->draw(shaderTerrain, *light, glm::vec4(0.0f, 1.0f, 0.0f, -water->getHeight()+1.0f));
			skybox->draw(shaderSkybox, *light);
			//billboard->draw(shaderBill, *light);
			fire->draw(shaderFire, dt);
			water->endReflection();
			m_Renderer.endScene();

			m_InvertCamera->moveHeight(distance);

			m_Renderer.beginScene(m_Camera);
			water->startRefraction();
			m_Renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);
			//animatedModel->draw(shaderModel, *light, ts, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
			terrain->draw(shaderTerrain, *light, glm::vec4(0.0f, -1.0f, 0.0f, water->getHeight()));
			skybox->draw(shaderSkybox, *light);
			//billboard->draw(shaderBill, *light);
			fire->draw(shaderFire, dt);
			water->endRefraction();
			m_Renderer.endScene();

			m_Renderer.beginScene(m_Camera);

			m_Renderer.viewport(m_Window);

			m_Renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);
			skybox->draw(shaderSkybox, *light);
			terrain->draw(shaderTerrain, *light);
			animatedModel->draw(shaderAnimatedModel, *light, ts);
			fire->draw(shaderFire, dt);
			//model->draw(shaderModel, *light);
			//billboard->draw(shaderBill, *light, dt);
			shaderGrass->bind();
			shaderGrass->sendMat4("u_Model", glm::mat4(1.0f));
			grass->draw(shaderGrass, *light, dt);
			water->draw(shaderWater, *light);

			shaderCubeMap->bind();
			shaderCubeMap->sendInt1("u_CubeMap", texture->bind(1));
			model->draw(shaderCubeMap, *light);

			//m_Camera->setSensitivity(124.44444f);
			//m_Camera->getPerspectiveCameraPropsRef().fov = 7.0f;
			//m_InvertCamera->getPerspectiveCameraPropsRef().fov = 7.0f;
			//spyGlassZoom->draw(shaderZoom);

			skybox->updateRotation(3.0f * dt);

			if (imGuiActive)
			imGui->render();

			m_Window->update();

			LARGE_INTEGER endTime;
			QueryPerformanceCounter(&endTime);
			dt = (float)(endTime.QuadPart - beginTime.QuadPart) / freq;
			beginTime = endTime;
		}

	}

	void Scene3D::windowResizeEvent(WindowResizeEvent e) {

	}

	void Scene3D::windowCloseEvent(WindowCloseEvent e) {

		m_Running = false;

	}

	void Scene3D::windowMoveEvent(WindowMoveEvent e) {

	}

	void Scene3D::mouseMoveEvent(MouseMoveEvent e) {

		//m_Camera->move(m_Window->getHandler(), e);
		//m_InvertCamera->move(m_Window->getHandler(), e);

		//Log::info("%s", e.toString().c_str());
		//Log::info("%.4f , %.4f",(e.getX() / m_Window->getWidth()) * 2.0f - 1.0f, 
		//	(1 - e.getY() / m_Window->getHeight()) * 2.0f - 1.0f); // Window coordinates to Normalized device coordinate system conversion

	}

	void Scene3D::keyEvent(KeyEvent e) {

		if (e.getKeyCode() == GLFW_KEY_ESCAPE && e.getAction() == GLFW_PRESS) {
			m_Running = false;
		}

		if (e.getKeyCode() == GLFW_KEY_O && e.getAction() == GLFW_PRESS) {
			saveImage("ejemplo.bmp", m_Window->getHandler());
		}

		if (e.getKeyCode() == GLFW_KEY_Y && e.getAction() == GLFW_PRESS) {
			imGuiActive = !imGuiActive;
		}

		if (e.getKeyCode() == GLFW_KEY_P && e.getAction() == GLFW_PRESS) { // Abstract this pls, no GLFW in Scene Class
			if (Input::getCursorMode(m_Window) == CursorMode::Normal) {
				Input::setCursorMode(m_Window, CursorMode::Disabled);
			}
			else {
				Input::setCursorMode(m_Window, CursorMode::Normal);
			}
		}

	}

	void Scene3D::joyStickEvent(JoyStickEvent e) {

		if (e.getEvent() == GLFW_CONNECTED) {
			Log::info("Connected");
		}
		else if (e.getEvent() == GLFW_DISCONNECTED) {
			Log::info("Disconnected");
		}

	}

	void Scene3D::imGuiEvent(bool a) {

		ImGui::Begin("Info");
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		//ImGui::SliderFloat3("Light Position:", &light->getPosition()[0], -100, 100);
		//ImGui::SliderFloat3("Light Colour:", &light->getColour()[0], 0, 1);
		ImGui::InputFloat3("Player Position:", &m_Camera->getPosition()[0]);
		ImGui::InputFloat3("Player Forward:", &m_Camera->getForward()[0]);
		ImGui::InputFloat3("Player Right:", &m_Camera->getRight()[0]);
		ImGui::InputFloat("Player Pitch:", &m_Camera->getPitch());
	    ImGui::InputFloat("Player Yaw:", &m_Camera->getYaw());
		//ImGui::SliderFloat3("Water Position:", &water->getPositionRef()[0], -20, 20);
		//ImGui::SliderFloat3("Water Scale:", &water->getScaleRef()[0], -20, 20);
		//ImGui::SliderFloat("Tiling Factor:", &terrain->getTilingFactorRef(), 0.0f, 100.0f);
		//ImGui::SliderFloat("Fall Velocity: ", &m_Camera->getUpVelocityRef(), 0.0f, 0.0f);
		//ImGui::PushID(0);
		//ImGui::SliderFloat4("Matriz: ", glm::value_ptr(joint), -1.0f, 1.0f);
		//ImGui::PopID();
		//ImGui::PushID(1);
		//ImGui::SliderFloat4("Matriz: ", glm::value_ptr(joint) + 4, -1.0f, 1.0f);
		//ImGui::PopID();
		//ImGui::PushID(2);
		//ImGui::SliderFloat4("Matriz: ", glm::value_ptr(joint) + 8, -1.0f, 1.0f);
		//ImGui::PopID();
		//ImGui::PushID(3);
		//ImGui::SliderFloat4("Matriz: ", glm::value_ptr(joint) + 12, -1.0f, 1.0f);
		//ImGui::PopID();
		ImGui::End();

	}

	void saveImage(std::string filepath, GLFWwindow* w) {
		int width, height;
		glfwGetFramebufferSize(w, &width, &height);
		GLsizei nrChannels = 3;
		GLsizei stride = nrChannels * width;
		stride += (stride % 4) ? (4 - stride % 4) : 0;
		GLsizei bufferSize = stride * height;
		std::vector<char> buffer(bufferSize);
		//glPixelStorei(GL_PACK_ALIGNMENT, 4);
		//glReadBuffer(GL_FRONT);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
		stbi_flip_vertically_on_write(true);
		stbi_write_png(filepath.c_str(), width, height, nrChannels, buffer.data(), stride);
	}
}