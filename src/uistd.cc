#include <cctype>
#include <cstdio>
#include "uistd.h"

edi::UiStd::UiStd() {
	enableRawMode();
}

edi::UiStd::~UiStd() {
	disableRawMode();
}

void edi::UiStd::enableRawMode() {
	tcgetattr(STDIN_FILENO, &_origTermios);
	_raw = _origTermios;
	_raw.c_lflag &= ~(ECHO | ICANON);

	tcsetattr(STDIN_FILENO, TCIFLUSH, &_raw);
}

void edi::UiStd::disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &_origTermios);
}

void edi::UiStd::run() {
	while (read(STDIN_FILENO, &_c, 1) == 1 && _c != _quitChar) {
		if (iscntrl(_c)) {
			printf("%d\n", _c);
		} else {
			printf("%d ('%c')\n", _c, _c);
		}
	}
}
