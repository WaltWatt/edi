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
	tcgetattr(STDIN_FILENO, &_origTermios);
	_raw = _origTermios;
	_raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	_raw.c_oflag &= ~(OPOST);
	_raw.c_cflag |= ~(CS8);
	_raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	_raw.c_cc[VMIN] = 0;
	_raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCIFLUSH, &_raw) == -1) {
		throw CException("UiTerm::enableRawMode(): tcsetattr() returned -1");
	}
}

void edi::Tui::disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_origTermios) == -1) {
		throw CException("UiTerm::disableRawMode(): tcsetattr() returned -1");
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

void edi::Tui::setQuitFlag(bool flag) {
	_quitFlag = flag;
}


int edi::Tui::exec()
{
	try {
		while(!_quitFlag) {
			handleKeyEvent();
		}
		printf("Bye! \r\n");
	} catch (CException e) {
		fprintf(stderr, "%s\r\n", e.what());
	}
	return 0;
}
