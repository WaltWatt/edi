#ifndef EDI_UISTD
#define EDI_UISTD

#include <termios.h>
#include <unistd.h>

namespace edi {
class UiStd {
	char _c;
	char _quitChar = 'q';
	struct termios _origTermios;
	struct termios _raw;

	void enableRawMode();
	void disableRawMode();
public:
	UiStd();
	~UiStd();

	void run();
}; // class UiStd
} // namespace edi

#endif // EDI_UISTD
