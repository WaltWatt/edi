#ifndef MODAL_WINDOW_H
#define MODAL_WINDOW_H

#include <memory>

#include "windowmode.h"

namespace edi
{
class ModalWindow
{
	WindowMode *_mode;

	std::unique_ptr<WindowNormalMode> _normalMode;
	std::unique_ptr<WindowInsertMode> _insertMode;
	std::unique_ptr<WindowCommandMode> _commandMode;

public:
	ModalWindow();
	virtual ~ModalWindow();

	void setMode(WindowMode mode)
	{
		*_mode = mode;
	}

	virtual void handleKeyEvent();
};

} // namespace edi
#endif // MODAL_WINDOW_H
