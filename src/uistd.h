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
	char _c;
	const char _quitChar;
	struct termios _origTermios;
	struct termios _raw;

	void enableRawMode();
	void disableRawMode();
	void die(const char *s);
public:
	UiStd();
	~UiStd();

	void run();
}; // class UiStd
} // namespace edi

#endif // EDI_UISTD
