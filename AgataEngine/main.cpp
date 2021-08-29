#include "pch.h"
#include "Scene3D.h"
#include <Windows.h>
#include <stb/stb_image.cpp>

//int Nomain() {
//
//	Window window = Window(1280, 720, "Mi ventana");
//	window.setVSync(true);
//
//	window.setIcons("images.jpg");
//
//	glViewport(0, 0, 1280, 720);
//
//	if (glewInit() != GLEW_OK) {
//		Log::error("ERROR no se pudo iniciar GLEW\n");
//		return -1;
//	}
//
//	ImGui::CreateContext();
//	ImGuiIO& io = ImGui::GetIO(); (void)io;
//	ImGui::StyleColorsDark();
//	ImGui_ImplGlfw_InitForOpenGL(window.getHandler(), true);
//	ImGui_ImplOpenGL3_Init("#version 330");
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//	glEnable(GL_DEPTH_TEST);
//
//	Renderer renderer;
//
//	auto [vertices, indices] = Loader::loadQuad();
//	Mesh mesh(vertices, indices);
//
//	//Model skull = Model("tea.obj"s, "barrel.png"s, "barrelSpecular.png"s, "barrelNormal.png"s, glm::vec3(6.10f, 2.4f, 11.12f), 
//	//		glm::vec3(0.0f), glm::vec3(1.0f));
//
//
//	Model skull = ModelBuilder().
//		ModelPath("Assets\\lantern.obj"s).
//		Position(glm::vec3(6.10f, 2.4f, 11.12f)).
//		Rotation(glm::vec3(0.0f)).
//		Scale(glm::vec3(10.0f)).
//		DiffuseTexture("Assets\\diffuse.jpg"s).
//		//SpecularTexture("Assets\\barrelSpecular.png"s).
//		NormalTexture("Assets\\normal.png"s).
//		AmbientMaterial(glm::vec3(0.75f)).
//		DiffuseMaterial(glm::vec3(0.85f)).
//		SpecularMaterial(glm::vec3(1.0f)).
//		ShininessMaterial(32).
//		Build();
//
//	Terrain terrain = TerrainBuilder().
//		RedTexture("Assets\\snow.jpg"s).
//		GreenTexture("Imagenes\\rock_vulcano.jpg"s).
//		BlueTexture("Assets\\dirt.jpg"s).
//		BlackTexture("Imagenes\\Zacate.png"s).
//		BlendMap("Mini_BlendMap.jpg"s).
//		HeightMap("Mini_HeightMap.jpg"s).
//		RedNormal("Assets\\snowNorm.jpg"s).
//		GreenNormal("Imagenes\\Rock_Vulcano_Normal.png"s).
//		BlueNormal("Assets\\dirtNorm.jpg"s).
//		BlackNormal("Imagenes\\ZacateNormal.png"s).
//		Build();
//
//	Skybox skybox("right.png", "left.png", "top.png", "bottom.png", "front.png", "back.png", 100.0f);
//
//	Shader shader("ModelVertex.glsl", "ModelFragment.glsl");
//	Shader shaderTerrain("TerrainVertex.glsl", "TerrainFragment.glsl");
//	Shader shaderSkybox("SkyBoxVertex.glsl", "SkyBoxFragment.glsl");
//	Shader shaderX("vertex.glsl", "fragment.glsl");
//	Shader shaderCubeMap("cubeMapVertex.glsl", "cubeMapFragment.glsl");
//
//	Billboard billboard("prueba.jpg", glm::vec3(11.302f, 1.918f, 12.408f), glm::vec3(1.0f));
//	//Billboard billboard("prueba.jpg");
//	Shader shaderBill("BillboardVS.glsl", "BillboardFS.glsl");
//	Shader shaderWater("WaterVertex.glsl", "WaterFragment.glsl");
//
//	Camera camera = Camera(1280, 720, 45.0f, 0.1f, 100.0f, 0.1f, 2.0f);
//	Camera invertCamera = camera;
//	invertCamera.changePitchDirection();
//	Water water(glm::vec3(5.0f, 1.11f, 4.658f), glm::vec3(4.0f, 1.0f, 5.0f), "dudv.png", "normalMap.png", 480, 360);
//
//	// 12, 20, 69
//	// Night 20-60-180
//	// dawn/dusk, linear fade to : 255, 60, 60 : 255, 128, 128
//	// Sun 255-255-220
//
//	Light light(glm::vec3(11.0f, 0.0f, -10.0f), glm::vec3(255.0f / 255.0f, 128.0f / 255.0f, 128.0f / 255.0f));
//	//Light light(glm::vec3(11.0f, 0.0f, -10.0f), glm::vec3(12.0f / 255.0f, 20.0f / 255.0f, 70.0f / 255.0f));
//
//	double prevTime = 0.0;
//	double currentTime = 0.0;
//	double diff = 0.0;
//	unsigned int counter = 0;
//
//	float rotate = 0.0f;
//	int up = 1;
//	int up2 = 1;
//
//	renderer.cullface(true);
//	float i = 180.0f;
//
//	while (!glfwWindowShouldClose(window.getHandler())) {
//
//		float r = glm::sqrt(1600 / (4 * pow(cos(glm::radians(i)), 2) + pow(sin(glm::radians(i)), 2)));
//		float x = r * cos(glm::radians(i));
//		float y = r * sin(glm::radians(i));
//
//		light.setPositionY(y * 0.5f);
//		light.setPositionZ(x + 10);
//
//		float intensity = glm::clamp(glm::sin(glm::radians(i)), 0.2f, 0.8f);
//
//		if (i >= 0) {
//			skull.getMaterialRef().setDiffuse(glm::vec3(intensity + 0.2f));
//			terrain.setDiffuse(glm::vec3(intensity + 0.2f));
//		}
//		else {
//			skull.getMaterialRef().setDiffuse(glm::vec3(0.0f));
//			terrain.setDiffuse(glm::vec3(0.0f));
//		}
//
//		skull.getMaterialRef().setAmbient(glm::vec3(intensity));
//		terrain.setAmbient(glm::vec3(glm::clamp(intensity, 0.2f, 0.4f)));
//
//		//(i > -180.0f) ? i -= 0.1f : i = 180.0f;
//		(i > 0) ? i -= 0.1f : i = 180.0f;
//
//		if (i < 160.0f && i > 120.0f) {
//		
//			light.setColourG(0.00125);
//			light.setColourB(0.00125);
//		
//		}
//		
//		if (i < 60.0f && i > 20.0f) {
//		
//			light.setColourG(-0.00125);
//			light.setColourB(-0.00125);
//		
//		}
//
//		Log::info("%.4f", i);
//
//		ImGui_ImplOpenGL3_NewFrame();
//		ImGui_ImplGlfw_NewFrame();
//		ImGui::NewFrame();
//
//		renderer.viewport(window.getHandler());
//
//		currentTime = glfwGetTime();
//		diff = currentTime - prevTime;
//		counter++;
//		if (diff >= 1.0 / 30.0) {
//			prevTime = currentTime;
//			counter = 0;
//		}
//
//		ImGui::Begin("Info");
//		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
//		ImGui::SliderFloat3("Light Position:", &light.getPosition()[0], -100, 100);
//		ImGui::SliderFloat3("Light Colour:", &light.getColour()[0], 0, 1);
//		ImGui::SliderFloat3("Player Position:", &camera.getPosition()[0], 0, 20);
//		ImGui::SliderFloat3("Player up:", &camera.getForward()[0], -1, 1);
//		//ImGui::SliderFloat3("Player forward:", &camera.getForward()[0], 0, 0);
//		ImGui::SliderFloat3("Water Position:", &water.getPositionRef()[0], -20, 20);
//		ImGui::SliderFloat3("Water Scale:", &water.getScaleRef()[0], -20, 20);
//		glm::vec3 vecPos = billboard.getPositionRef() - camera.getPosition();
//		ImGui::Text("Arctan %.4f", glm::degrees(glm::atan(vecPos.x / vecPos.z)));
//		ImGui::End();
//
//		invertCamera.move(window.getHandler(), terrain, diff * 20.0f);
//		invertCamera.update();
//		camera.move(window.getHandler(), terrain, diff * 20.0f);
//		camera.update();
//
//		glEnable(GL_CLIP_DISTANCE0);
//
//#if 1
//		float distance = 2 * (camera.getPosition().y - water.getHeight());
//		invertCamera.moveHeight(-distance);
//		renderer.beginScene(invertCamera);
//			water.startReflection();
//			renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);
//			skull.draw(shader, light, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
//			terrain.draw(shaderTerrain, light, glm::vec4(0.0f, 1.0f, 0.0f, -water.getHeight()));
//			skybox.draw(shaderSkybox, light);
//			billboard.draw(shaderBill, light);
//			water.endReflection();
//		renderer.endScene();
//
//		invertCamera.moveHeight(distance);
//
//		renderer.beginScene(camera);
//			water.startRefraction();
//			renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);
//			skull.draw(shader, light, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
//			terrain.draw(shaderTerrain, light, glm::vec4(0.0f, -1.0f, 0.0f, water.getHeight()));
//			skybox.draw(shaderSkybox, light);
//			billboard.draw(shaderBill, light);
//			water.endRefraction();
//		renderer.endScene();
//
//
//		renderer.beginScene(camera);
//
//		renderer.viewport(window.getHandler());
//
//		renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);
//		shader.bind();
//		skull.draw(shader, light, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
//		terrain.draw(shaderTerrain, light, glm::vec4(0.0f, 0.0f, 0.0f, 0.0f));
//		skybox.draw(shaderSkybox, light);
//		billboard.draw(shaderBill, light);
//
//		water.draw(shaderWater, light);
//
//#else
//
//		renderer.beginScene(camera);
//		renderer.viewport(window);
//		renderer.clear(0.1f, 0.1f, 0.1f, 1.0f);
//		skybox.draw(shaderSkybox, light);
//
//		shaderCubeMap.bind();
//		shaderCubeMap.sendMat4("u_Model", glm::mat4(1.0f));
//		shaderCubeMap.sendMat4("u_View", Renderer::getView());
//		shaderCubeMap.sendMat4("u_Projection", Renderer::getProjection());
//		shaderCubeMap.sendMat4("u_RotCubeMap", glm::rotate(glm::mat4(1.0f), glm::radians(180 + skybox.getRotation()), glm::vec3(0, 1, 0)));
//		shaderCubeMap.sendVec3("u_CameraPos", camera.getPosition());
//		shaderCubeMap.sendVec3("u_LightPos", light.getPosition());
//		shaderCubeMap.sendVec3("u_LightColour", light.getColour());
//		skybox.active();
//		shaderCubeMap.sendInt1("u_CubeMap", 0);
//		skull.draw();
//
//
//#endif
//
//		//shaderX.bind();
//		//glActiveTexture(GL_TEXTURE0);
//		//glBindTexture(GL_TEXTURE_2D, refraction.getTexture());
//		//shaderX.sendInt1("u_Refraction", 0);
//		//shaderX.sendMat4("u_View", Renderer::getView());
//		//shaderX.sendMat4("u_Projection", Renderer::getProjection());
//		////glActiveTexture(GL_TEXTURE1);
//		////glBindTexture(GL_TEXTURE_2D, reflection.getTexture());
//		//Renderer::drawIndex(mesh.getVertexArray(), mesh.getIndexCount());
//		//renderer.endScene();
//
//		skybox.updateRotation(0.05f);
//		
//		ImGui::Render();
//		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//		window.update();
//	}
//
//	ImGui_ImplOpenGL3_Shutdown();
//	ImGui_ImplGlfw_Shutdown();
//	ImGui::DestroyContext();
//
//}
#include "Random.h"

int main(int argc, char** argv) {

	Agata::Random::init();

	while (1) {
		std::cout << Agata::Random::Float() << std::endl;
		Sleep(1000);
	}

	Agata::Scene* scene = new Agata::Scene3D();
	scene->run();
	delete scene;

}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int nCmdShow) {

	Agata::Scene* scene = new Agata::Scene3D();
	scene->run();
	delete scene;

}