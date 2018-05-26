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

	int res = ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
	if (res == -1) {
		throw CException("Tui::getWindowSize(): ioctl() returned -1 ");
	}
	if (ws.ws_col == 0) {
		throw CException("Tui::getWindowSize(): ws.ws_col == 0");
	}
	*cols = ws.ws_col;
	*rows = ws.ws_row;
}

void edi::Tui::initEdi() {
	try {
		getWindowSize(&_e.screenrows, &_e.screencols);
	} catch (CException e) {
		fprintf(stderr, "***ERROR: %s. Exiting!\r\n", e.what());
		exit(-1);
	}
}

// ToDo: should go to a class called KeyEvent (or something)
char edi::Tui::readKey() const
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			throw CException("UiTerm::readKey(): read() returned -1");
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
	initEdi();
	try {
		while(!_quitFlag) {
			refreshScreen();
			handleKeyEvent();
		}
		cleanScreen();
		printf("Edi says: Bye!\r\n");
	} catch (CException e) {
		fprintf(stderr, "***ERROR: %s. Exiting!\r\n", e.what());
		exit(-1);
	}
	return 0;
}
