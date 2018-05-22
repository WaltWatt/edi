#include "uiterm.h"
#include "cexception.h"

edi::UiTerm::UiTerm() 
	: _quitFlag(false),
	  _normalMode(new UiModeNormal()),
	  _commandMode(new UiModeCommand()),
	  _insertMode(new UiModeInsert()),
 	  _mode(_normalMode)
{
	enableRawMode();
}

edi::UiTerm::~UiTerm()
{
	disableRawMode();

	delete _normalMode;
	delete _commandMode;
	delete _insertMode;
}

void edi::UiTerm::enableRawMode()
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

void edi::UiTerm::disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_origTermios) == -1) {
		throw CException("UiTerm::disableRawMode(): tcsetattr() returned -1");
	}
}

void edi::UiTerm::setNormalMode()
{
	_mode = _normalMode;
}

void edi::UiTerm::setInsertMode()
{
	_mode = _insertMode;
}

void edi::UiTerm::setCommandMode()
{
	_mode = _commandMode;
}

void edi::UiTerm::setQuitFlag(bool flag) {
	_quitFlag = flag;
}

// ToDo: should go to a class called KeyEvent (or something)
char edi::UiTerm::readKey() const
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

int edi::UiTerm::exec()
{
	try {
		while(!_quitFlag) {
			char c = readKey();
			_mode->processKeyboardEvent(c, this);
		}
		printf("Bye! \r\n");
	} catch (CException e) {
		fprintf(stderr, "%s\r\n", e.what());
	}
	return 0;
}
