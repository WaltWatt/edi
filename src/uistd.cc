#include "uistd.h"

edi::UiStd::UiStd() 
	: _normalMode(new UiModeNormal()),
	  _commandMode(new UiModeCommand()),
	  _insertMode(new UiModeInsert()),
 	  _mode(_normalMode)
{
	enableRawMode();
}

edi::UiStd::~UiStd()
{
	disableRawMode();

	delete _normalMode;
	delete _commandMode;
	delete _insertMode;
}

void edi::UiStd::enableRawMode()
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
		die("UiStd::enableRawMode(): returned -1", 1);
	}
}

void edi::UiStd::disableRawMode()
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_origTermios) == -1) {
		die("UiStd::disableRawMode(): returned -1", 1);
	}
}

void edi::UiStd::setNormalMode()
{
	_mode = _normalMode;
}

void edi::UiStd::setInsertMode()
{
	_mode = _insertMode;
}

void edi::UiStd::setCommandMode()
{
	_mode = _commandMode;
}

// ToDo: should go to a class called KeyEvent (or something)
char edi::UiStd::readKey() const
{
	int nread;
	char c;
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			die("UiStd::exec(): read(): returned -1", 1);
		}
	}
	return c;
}

// ToDo: replace with throwing exception(?)
void edi::UiStd::die(const char *msg, int exitCode) const
{
	fprintf(exitCode ? stderr : stdout, "%s", msg);
	exit(exitCode);
}

int edi::UiStd::exec()
{
	while(1) {
		char c = readKey();
		_mode->processKeyboardEvent(c, this);

		// ToDo: commands to be processed by _commandMode
		//       which should send some sort of exit signal to ui
		if (_mode == _commandMode && c == 'q') {
			printf("Bye! \r\n");
			break;
		}
	}
	return 0;
}
