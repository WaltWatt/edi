#ifndef WINDOW_MODE_H
#define WINDOW_MODE_H

namespace edi
{
class ModalWindow;
class KeyEvent;

class WindowMode
{
public:
	WindowMode();
	virtual ~WindowMode(){}

	virtual void processKeyEvent(KeyEvent event, ModalWindow window) = 0;
};

class WindowNormalMode : public WindowMode
{
public:
	WindowNormalMode();
	virtual ~WindowNormalMode(){}

	void processKeyEvent(KeyEvent event, ModalWindow window);
};

class WindowInsertMode : public WindowMode
{
public:
	WindowInsertMode();
	virtual ~WindowInsertMode(){}

	void processKeyEvent(KeyEvent event, ModalWindow window);
};

class WindowCommandMode : public WindowMode
{
public:
	WindowCommandMode();
	virtual ~WindowCommandMode(){}

	void processKeyEvent(KeyEvent event, ModalWindow window);
};
} // namespace edi
#endif // WINDOW_MODE_H
