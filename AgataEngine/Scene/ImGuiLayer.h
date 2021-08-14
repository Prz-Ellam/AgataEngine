#ifndef IM_GUI_LAYER_H
#define IM_GUI_LAYER_H

#include "Window.h"
#include <functional>

class ImGuiLayer {
public:
	ImGuiLayer(Window* windowHost);
	void newFrame();
	void draw();
	void render();
	~ImGuiLayer();
	void attachDrawHandler(std::function<void(bool)> handler);
private:
	std::function<void(bool)> drawHandler;
};

#endif