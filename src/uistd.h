#ifndef EDI_UISTD
#define EDI_UISTD

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

class UiStd {
	UiModeNormal *_normalMode;
	UiModeCommand *_commandMode;
	UiModeInsert *_insertMode;

	UiMode *_mode;

	char _c;
	int _exitCode;
	//const char _quitChar;
	struct termios _origTermios;
	struct termios _raw;

	void exit(const char *msg, int exitCode = 0);

	void enableRawMode();
	void disableRawMode();

public:
	UiStd();
	~UiStd();

	void handleKeyEvent();
	void setNormalMode();
	void setInsertMode();
	void setCommandMode();

	int exec();

	void quit();
}; // class UiStd
} // namespace edi

#endif // EDI_UISTD
