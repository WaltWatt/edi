#ifndef UI_MODE_H
#define UI_MODE_H

#include "tui.h"

namespace edi {
class Tui;

class UiMode
{
public:
	UiMode(){}
	virtual ~UiMode() {}

	virtual void processKeyboardEvent(char c, Tui *ui) = 0;
}; 


class UiModeNormal : public UiMode
{
public:
	UiModeNormal();
	virtual ~UiModeNormal();

	void processKeyboardEvent(char c, Tui *ui);
}; 


class UiModeInsert : public UiMode
{
public:
	UiModeInsert();
	virtual ~UiModeInsert();

	void processKeyboardEvent(char c, Tui *ui);
}; 


class UiModeCommand : public UiMode
{
public:
	UiModeCommand();
	virtual ~UiModeCommand();

	void processKeyboardEvent(char c, Tui *ui);
}; 

} // namespace edi
#endif // UI_MODE_H
