#ifndef TUI_H
#define TUI_H

#include <cerrno>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <termios.h>
#include <unistd.h>
#include <string>
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

	std::unique_ptr<UiModeNormal> _normalMode;
	std::unique_ptr<UiModeInsert> _insertMode;
	std::unique_ptr<UiModeCommand> _commandMode;

	// A raw pointer here so I can use unique_ptr's rather then shared_ptr's
	// for modes. Will never call deleate on _mode. I hope.
	
	UiMode *_mode;

	void cleanScreen();
	void refreshScreen();
	//void drawRows(std::string ab);
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
