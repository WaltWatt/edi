#ifndef TUI_TMAINWINDOW_H
#define TUI_TMAINWINDOW_H

#include <system_error>
#include <sys/ioctl.h>
#include <unistd.h>

#include <cerrno>
#include <cstdio>

#include "TSize.h"

namespace tui
{
class TMainWindow
{
	struct winsize _size;

	void getWindowSize();
	TSize getCursorPosition();

public:
	TMainWindow();

	int columns() const;
	int rows() const;
};
} // namespace tui
#endif // TUI_TMAINWINDOW_H
