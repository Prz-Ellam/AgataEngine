#include "ImGuiLayer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

ImGuiLayer::ImGuiLayer(Window* windowHost) {

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(windowHost->getHandler(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

}

ImGuiLayer::~ImGuiLayer() {

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

void ImGuiLayer::newFrame() {

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

}

void ImGuiLayer::draw() {

	drawHandler(false);

}

void ImGuiLayer::render() {

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void ImGuiLayer::attachDrawHandler(std::function<void(bool)> handler) {

	drawHandler = handler;

}