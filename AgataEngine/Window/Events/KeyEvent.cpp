#include "KeyEvent.h"

KeyEvent::KeyEvent(int keyCode, int action) : m_KeyCode(keyCode), m_Action(action) {

}

KeyEvent::KeyEvent() : m_KeyCode(0), m_Action(0) {

}

int KeyEvent::getKeyCode() const {

	return m_KeyCode;

}

int KeyEvent::getAction() const {

	return m_Action;

}