#ifndef TUI_H
#define TUI_H

#include <cerrno>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>

#include "uimode.h"

namespace edi {
	class UiModeNormal;
	class UiModeCommand;
	class UiModeInsert;
	class UiMode;

class Tui
{
	bool _quitFlag;

	struct termios _origTermios;
	struct termios _raw;
	
	void enableRawMode();
	void disableRawMode();

	UiModeNormal *_normalMode;
	UiModeCommand *_commandMode;
	UiModeInsert *_insertMode;

	UiMode *_mode;

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
