#ifndef AGATA_KEY_EVENT_H
#define AGATA_KEY_EVENT_H

class KeyEvent {
public:
	KeyEvent(int keyCode, int action);
	KeyEvent();

	int getKeyCode() const;
	int getAction() const;

private:
	int m_KeyCode;
	int m_Action;
};

#endif