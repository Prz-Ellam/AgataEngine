#include "JoyStickEvent.h"


JoyStickEvent::JoyStickEvent(int ID, int event) : m_ID(ID), m_Event(event) {

}

int JoyStickEvent::getID() const {

	return m_ID;

}

int JoyStickEvent::getEvent() const {

	return m_Event;

}