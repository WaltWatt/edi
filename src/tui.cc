#include "tui.h"
#include "cexception.h"

edi::Tui::Tui() 
	: _quitFlag(false),
	  _normalMode(new UiModeNormal()),
	  _commandMode(new UiModeCommand()),
	  _insertMode(new UiModeInsert()),
 	  _mode(_normalMode)
{
	enableRawMode();
}

edi::Tui::~Tui()
{
	disableRawMode();
	delete _normalMode;
	delete _commandMode;
	delete _insertMode;
}

void edi::Tui::enableRawMode()
{
	if (tcgetattr(STDIN_FILENO, &_e.origTermios) == -1) {
		throw CException("Tui::enableRawMode(): tcsetattr() returned -1");
	}
	struct termios raw = _e.origTermios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= ~(CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCIFLUSH, &raw) == -1) {
		throw CException("Tui::enableRawMode(): tcsetattr() returned -1");
	}
}

void edi::Tui::disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_e.origTermios) == -1) {
		throw CException("Tui::disableRawMode(): tcsetattr() returned -1");
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

void edi::Tui::setNormalMode()
{
	_mode = _normalMode;
}

void edi::Tui::setInsertMode()
{
	_mode = _insertMode;
}

void edi::Tui::setCommandMode()
{
	_mode = _commandMode;
}

void edi::Tui::handleKeyEvent()
{
	_mode->processKeyboardEvent(readKey(), this);
}

void edi::Tui::cleanScreen() {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);
}

void edi::Tui::drawRows() {
	int y;
	for (y = 0; y < _e.screenrows - 2; y++) {
		write(STDOUT_FILENO, "~\r\n", 3);
	}
	write(STDOUT_FILENO, "NORMAL | <Esc>:q<Enter> to quit\r\n", 34);
}

void edi::Tui::refreshScreen() {
	cleanScreen();
	drawRows();
	write(STDOUT_FILENO, "\x1b[H", 3);
}

void edi::Tui::setQuitFlag(bool flag) {
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
