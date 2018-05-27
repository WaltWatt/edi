#ifndef TUI_H
#define TUI_H

#include <cerrno>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include "uimode.h"

namespace edi {
	class UiModeNormal;
	class UiModeCommand;
	class UiModeInsert;
	class UiMode;

class Tui
{
	bool _quitFlag;
	bool _inRawMode;

	void enableRawMode();
	void disableRawMode();

	struct Screen {
		int screenrows;
		int screencols;
		struct termios origTermios;
	};

	Screen _e;

	UiModeNormal *_normalMode;
	UiModeCommand *_commandMode;
	UiModeInsert *_insertMode;

	UiMode *_mode;

	void cleanScreen();
	void refreshScreen();
	void drawRows();

	void getWindowSize(int *rows, int *cols);
	void getCursorPosition(int *rows, int *cols);

	void initEdi();

	// ToDo: should go to a class called KeyEvent (or something)
	char readKey() const;

public:
	Tui();
	~Tui();

	void handleKeyEvent();
	void setNormalMode();
	void setInsertMode();
	void setCommandMode();

	void setQuitFlag(bool flag);
	int exec();
}; // class Tui
} // namespace edi
#endif // TUI_H
