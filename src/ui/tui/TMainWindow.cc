#include "TMainWindow.h"

#include <exception>
#include <iostream>
using namespace std;

tui::TMainWindow::TMainWindow()
{
	try {
		getWindowSize();
	} catch (system_error &e) {
		fprintf(stderr, "TMainWindow::TMainWindow(): %s\n",
				        e.what());
		throw;
	} catch (runtime_error &e) {
		fprintf(stderr, "TMainWindow::TMainWindow(): %s\n",
				        e.what());
		throw;
	}
}

int tui::TMainWindow::cols() const
{
	return _size.ws_col;
}

void tui::TMainWindow::getWindowSize()
{
	int ioctl_retv = ioctl(STDIN_FILENO, TIOCGWINSZ, &_size);
	if (ioctl_retv == -1 || _size.ws_col == 0) {
	//if (1 || ioctl_retv == -1 || _size.ws_col == 0) {
		int write_retv = write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12); 
		if (write_retv != 12) {
			error_code ec(write_retv, generic_category());
			system_error e(ec, "getWindowSize(): write()");
			throw e;
		}

		try {
			pair<int, int> size = getCursorPosition();
			_size.ws_row = size.first;
			_size.ws_col = size.second;
		} catch (runtime_error &e) {
			throw; 
		}

		if (ioctl_retv == -1) {
			error_code ec(ioctl_retv, generic_category());
			throw system_error (ec, "getWindowSize(): ioctl()");
		}

		if (_size.ws_col == 0) {
			throw runtime_error ("getWindowSize(): cols == 0");
		}
	}
}

pair<int, int> tui::TMainWindow::getCursorPosition()
{

	char buf[32];
	unsigned int i = 0;
	pair<int, int> size;

	int write_retv = write(STDOUT_FILENO, "\x1b[6n", 4);
	if (write_retv != 4) {
		error_code ec(write_retv, generic_category());
		throw system_error(ec, "getCursorPosition(): write()");
	}

	while (i < sizeof(buf) - 1) {
	/*
	    // ToDo: maybe the terminal has to be in row mode
		// has to be tested?!
		
		if (read(STDIN_FILENO, &buf[i], 1) != 1) {
			break;
		}
		if (buf[i] == 'R') {
			break;
		}
	*/
		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[') {
		throw runtime_error("getCursorPosition()");
	}
	int sscanf_retv = sscanf(&buf[2], "%d;%d", &size.first, &size.second);
	printf("\n%d;%d\n", size.first, size.second);
	if (sscanf_retv != 2) {
		error_code ec(sscanf_retv, generic_category());
		throw system_error(ec, "getCursorPosition(): sscanf()");
	}

	return size;
}


