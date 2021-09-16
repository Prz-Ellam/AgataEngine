#ifndef AGATA_JOY_STICK_EVENT_H
#define AGATA_JOY_STICK_EVENT_H

class JoyStickEvent {
public:
	JoyStickEvent() = default;
	JoyStickEvent(int ID, int event);
	virtual ~JoyStickEvent() = default;

	int getID() const;
	int getEvent() const;

private:
	int m_ID, m_Event;
};

#endif