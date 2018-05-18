#ifndef EDI_UISTD
#define EDI_UISTD

#include <cerrno>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>

namespace edi {
class UiStd {
	enum mode {MODE_NORMAL, MODE_INSERT, MODE_COMMAND};
	mode _mode;
	char _c;
	int _exitCode;
	const char _quitChar;
	struct termios _origTermios;
	struct termios _raw;
	void exit(const char *msg, int exitCode = 0);

	void enableRawMode();
	void disableRawMode();

public:
	UiStd();
	~UiStd();

	int exec();
}; // class UiStd
} // namespace edi

#endif // EDI_UISTD
