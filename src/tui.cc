#include <stdexcept>

#include "tui.h"

using namespace std;

edi::Tui::Tui() 
	: _quitFlag(false),
	  _inRawMode(false),
	  _normalMode(new UiModeNormal()),
	  _insertMode(new UiModeInsert()),
	  _commandMode(new UiModeCommand()),
 	  _mode(_normalMode.get())
{
	printf("Tui() called\n");
	enableRawMode();
}

edi::Tui::~Tui()
{
	disableRawMode();
	printf("~Tui() called\n");
}

void edi::Tui::enableRawMode()
{
	if (!_inRawMode) {
		if (tcgetattr(STDIN_FILENO, &_e.origTermios) == -1) {
			throw runtime_error(
					"Tui::enableRawMode(): "\
					"tcsetattr(tcgetattr(STDIN_FILENO, &_e.origTermios) "\
					"returned -1");
		}
		struct termios raw = _e.origTermios;
		raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
		raw.c_oflag &= ~(OPOST);
		raw.c_cflag |= ~(CS8);
		raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
		raw.c_cc[VMIN] = 0;
		raw.c_cc[VTIME] = 1;

		if (tcsetattr(STDIN_FILENO, TCIFLUSH, &raw) == -1) {
			throw runtime_error(
					"Tui::enableRawMode(): "\
					"tcsetattr(STDIN_FILENO, TCIFLUSH, &raw) "\
					"returned -1");
		}
		_inRawMode = true;
	}
}

void edi::Tui::disableRawMode()
{
	if (_inRawMode) {
		if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_e.origTermios) == -1) {
			throw runtime_error(
					"Tui::disableRawMode(): tcsetattr() returned -1");
		}
		_inRawMode = false;
	}
}

void edi::Tui::getCursorPosition(int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;

	if (write(STDERR_FILENO, "\x1b[6n", 4) != 4) {
		throw runtime_error(
				"Tui::getCursorPosition(): write() did not return 4");
	}

	while (i < sizeof(buf) - 1) {
		if (read(STDERR_FILENO, &buf[i], 1) != 1) {
			break;
		}
		if (buf[i] == 'R') {
			break;
		}
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[') {
		throw runtime_error("Tui::getCursorPosition(): "\
				            "buf[0] != '\x1b' || buf[1] != '['");
	}
	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
		throw runtime_error(
				"Tui::getCursorPosition(): sscanf() did not returne 2");
	}
}

void edi::Tui::getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
			throw runtime_error(
					"Tui::getWindowSize(): write() did not return 12");
		}
		try {
			getCursorPosition(rows, cols);
		} catch (runtime_error &e) {
			throw runtime_error (std::string("Tui::getWindowSize():\n -> ")
					             + e.what());
		}
	} else {
		*cols = ws.ws_col;
		*rows = ws.ws_row;
	}
}

void edi::Tui::initEdi() {
	try {
		getWindowSize(&_e.screenrows, &_e.screencols);
	} catch (std::exception &e) {
		throw runtime_error (std::string("Tui::initEdi():\n -> ") + e.what());
	}
}

// ToDo: should go to a class called KeyEvent (or something)
char edi::Tui::readKey() const
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			throw runtime_error("UiTerm::readKey(): read() returned -1");
		}
	}
	return c;
}

/*
void edi::Tui::setUiMode(std::unique_ptr<edi::UiMode> mode) {
	_mode = mode.get();
}
*/

void edi::Tui::setNormalMode()
{
	_mode = _normalMode.get();
}

void edi::Tui::setInsertMode()
{
	_mode = _insertMode.get();
}

void edi::Tui::setCommandMode()
{
	_mode = _commandMode.get();
}

void edi::Tui::handleKeyEvent()
{
	_mode->processKeyboardEvent(readKey(), this);
}

void edi::Tui::cleanScreen() {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

void edi::Tui::drawRows(std::string &ab) {
	int y;
	for (y = 0; y < _e.screenrows - 2; y++) {
		if (y == _e.screenrows / 3) {
			// ToDo: remove the literal out of here:
			std::string welcome = "Edi -- version ";
			// ToDo: remove the literal out of here:
			welcome.append("0.0.1");
			int welcomelen = welcome.size();
			if (welcomelen > _e.screencols) {
				welcomelen = _e.screencols;
			}
			int padding = (_e.screencols - welcomelen) / 2;
			if (padding) {
				ab.append("~");
				padding--;
			}
			while (padding--) {
				ab.append(" ");
			}
			ab.append(welcome + "\x1b[K\r\n");
		} else {
			ab.append("~\x1b[K\r\n");
		}
	}
}

void edi::Tui::drawStatusBar(std::string &ab)
{
	ab.append("\x1b[1;7m");
	int len = 0;
	std::string mode = " " + _mode->toString();
	len += mode.size();
	mode.append("\x1b[0;7m");
	std::string msg = _mode->getMessage() + " ";
	len += msg.size();
	while (len < _e.screencols) {
		mode.append(" ");
		len++;
	}
	mode.append(msg);
	ab.append(mode);
	ab.append("\x1b[m");
}

void edi::Tui::refreshScreen()
{
	std::string ab;
	ab.append("\x1b[?25l");
	ab.append("\x1b[H");

	drawRows(ab);
	drawStatusBar(ab);

	ab.append("\x1b[H");
	ab.append("\x1b[?25h");
std::ios_base::failure& exceptio
	write(STDOUT_FILENO, ab.c_str(), ab.size());
}

void edi::Tui::setQuitFlag(bool flag)
{
	_quitFlag = flag;
}

int edi::Tui::exec()
{
	try {
		initEdi();
	} catch (std::runtime_error &e) {
		disableRawMode();
		fprintf(stderr, "\n*** Tui::exec():\n" " -> %s\n", e.what());
		return(-1);
	}
	try {
		while(!_quitFlag) {
			refreshScreen();
			handleKeyEvent();
		}
		cleanScreen();
		printf("Edi says: Bye!\r\n");
	} catch (runtime_error &e) {
		disableRawMode();
		fprintf(stderr, "\n*** Tui::Exec():\r\n -> %s\n", e.what());
		return(-1);
	} catch (...) {
		disableRawMode();
		fprintf(stderr, "\n*** Tui::Exec(): unnknown exception\n");
		return(-1);
	}
	return 0;
}
