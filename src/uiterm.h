#ifndef UITERM_H
#define UITERM_H

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

class UiTerm
{
	bool _quitFlag;

	UiModeNormal *_normalMode;
	UiModeCommand *_commandMode;
	UiModeInsert *_insertMode;

	UiMode *_mode;

	struct termios _origTermios;
	struct termios _raw;

	void enableRawMode();
	void disableRawMode();
	
	// ToDo: should go to a class called KeyEvent (or something)
	char readKey() const;

public:
	UiTerm();
	~UiTerm();

	void handleKeyEvent();
	void setNormalMode();
	void setInsertMode();
	void setCommandMode();

	void setQuitFlag(bool flag);

	int exec();
}; // class UiTerm
} // namespace edi

#endif // UITERM_H
