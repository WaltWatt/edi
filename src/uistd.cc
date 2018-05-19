#include "uistd.h"

edi::UiStd::UiStd() : _mode(MODE_NORMAL), _quitChar('q')  {
	enableRawMode();
}

edi::UiStd::~UiStd() {
	disableRawMode();
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
	//int exitCode = 0;
	while (1) {
		_c = '\0';
		if (read(STDIN_FILENO, &_c, 1) == -1 && errno != EAGAIN) {
			exit("UiStd::exec(): read(): returned -1", 1);
		}
		if (_mode == MODE_NORMAL) {
			printf("In 'normal' mode: ");
			if (_c == 'i') {
				_mode = MODE_INSERT;
			} 
			if (_c == ':') {
				_mode = MODE_COMMAND;
			}
		} else if (_mode == MODE_INSERT) {
			printf("In 'insert' mode: ");
			if (_c == 27) {
				_mode = MODE_NORMAL;
			}
		} else if (_mode == MODE_COMMAND) {
			printf("In 'command' mode: ");
			if (_c == 27) {
				_mode = MODE_NORMAL;
			}
			if (_c == _quitChar) {
				printf("%d ('%c')\r\n", _c, _c);
				break;
			}
		}
		if (iscntrl(_c)) {
			printf("%d\r\n", _c);
		} else {
			printf("%d ('%c')\r\n", _c, _c);
		}
	}
	return 0;
}

