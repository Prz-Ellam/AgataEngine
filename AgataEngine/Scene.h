#ifndef SCENE_H
#define SCENE_H

#include "Window.h"
#include "Renderer.h"

#define EVENT_FN(name) std::bind(&##name, this, std::placeholders::_1)

class Scene {
public:
	Scene();
	virtual ~Scene();

	virtual void init();
	virtual void shutdown();
	virtual void setCallbacks();
	virtual void run();

	virtual void windowResizeEvent(WindowResizeEvent e);
	virtual void windowCloseEvent(WindowCloseEvent e);
	virtual void windowMoveEvent(WindowMoveEvent e);
	virtual void mouseMoveEvent(MouseMoveEvent e);
	virtual void imGuiEvent(bool a);
protected:
	Window* m_Window;
	Renderer m_Renderer;
	bool m_Running;
};


#endif