#include "uistd.h"

edi::UiStd::UiStd() 
	: _normalMode(new UiModeNormal()),
	  _commandMode(new UiModeCommand()),
	  _insertMode(new UiModeInsert()),
 	  _mode(_normalMode)
{
	enableRawMode();
}

edi::UiStd::~UiStd() {
	disableRawMode();
	delete _normalMode;
	delete _commandMode;
	delete _insertMode;
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

void edi::UiStd::handleKeyEvent() {
	_c = '\0';
	if (read(STDIN_FILENO, &_c, 1) == -1 && errno != EAGAIN) {
		exit("UiStd::exec(): read(): returned -1", 1);
	}
}

// ToDo:
void edi::UiStd::quit()
{
}


void edi::UiStd::exit(const char *msg, int exitCode)
{
	fprintf(exitCode ? stderr : stdout, "%s", msg);
	_exitCode = exitCode;
}

void edi::UiStd::enableRawMode() {
	tcgetattr(STDIN_FILENO, &_origTermios);
	_raw = _origTermios;
	_raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	_raw.c_oflag &= ~(OPOST);
	_raw.c_cflag |= ~(CS8);
	_raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	_raw.c_cc[VMIN] = 0;
	_raw.c_cc[VTIME] = 1;

	if (tcsetattr(STDIN_FILENO, TCIFLUSH, &_raw) == -1) {
		exit("UiStd::enableRawMode(): returned -1", 1);
	}
}

void edi::UiStd::disableRawMode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_origTermios) == -1) {
		exit("UiStd::disableRawMode(): returned -1", 1);
	}
}

int edi::UiStd::exec() {
	while(1) {
		handleKeyEvent();
		_mode->processKeyboardEvent(_c, this);
		if (iscntrl(_c)) {
			printf("%d\r\n", _c);
		} else {
			printf("%d ('%c')\r\n", _c, _c);
		}
		if (_mode == _commandMode && _c == 'q') {
			printf("Bye! \r\n");
			break;
		}
	}
	return 0;
}
