#include "uistd.h"

edi::UiStd::UiStd() : _quitChar('q')  {
	enableRawMode();
}

edi::UiStd::~UiStd() {
	disableRawMode();
}

// ToDo: throw exception
void edi::UiStd::die(const char *s) {
	perror(s);
	exit(1);
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

	/*
	// ToDo: implement:
	try {
		tcsetattr(STDIN_FILENO, TCIFLUSH, &_raw);
	} catch (...) {
	}
	*/
	if (tcsetattr(STDIN_FILENO, TCIFLUSH, &_raw) == -1) {
		die("tcgetattr");
	}
}

void edi::UiStd::disableRawMode() {
	/*
	// ToDo: implement:
	try {
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &_origTermios);
	} catch (...) {
	}
	*/
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_origTermios) == -1) {
		die("tcsetattr");
	}
}

void edi::UiStd::run() {
	while (1) {
		_c = '\0';
		/*
		// ToDo: implement:
		try {
			read(STDIN_FILENO, &_c, 1);
		}
		catch (...) {
		}
		*/
		if (read(STDIN_FILENO, &_c, 1) == -1 && errno != EAGAIN) {
			die("read");
		}

		if (iscntrl(_c)) {
			printf("%d\r\n", _c);
		} else {
			printf("%d ('%c')\r\n", _c, _c);
		}
		if (_c == _quitChar) {
			break;
		}
	}
}

