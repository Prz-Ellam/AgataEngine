#include "Scene.h"
#include "Log.h"

Scene::Scene() {

}

Scene::~Scene() {

}

void Scene::init() {

}

void Scene::shutdown() {

}

void Scene::setCallbacks() {

	m_Window->setWindowCloseEventHandler(EVENT_FN(Scene::windowCloseEvent));

}

void Scene::run() {

	while (m_Running) {

		m_Window->update();

	}

}

void Scene::windowResizeEvent(WindowResizeEvent e) {

}

void Scene::windowCloseEvent(WindowCloseEvent e) {

	m_Running = false;

}

void Scene::windowMoveEvent(WindowMoveEvent e) {

}

void Scene::mouseMoveEvent(MouseMoveEvent e) {

}

void Scene::keyEvent(KeyEvent e) {

}

void Scene::imGuiEvent(bool a) {

}