#include "uimode.h"

edi::UiMode::UiMode(std::string name, std::string msg)
	: _mode(name),
	  _msg(msg)
{}

std::string edi::UiMode::toString()
{
	return _mode;
}

std::string edi::UiMode::getMessage()
{
	return _msg;
}


edi::UiModeNormal::UiModeNormal()
	:UiMode("NORMAL", "'i' -- INSERT mode | ':' -- COMMAND mode")
{
}

edi::UiModeNormal::~UiModeNormal()
{
}

void edi::UiModeNormal::processKeyboardEvent(char c, Tui *ui)
{
	if (c == 'i') {
		ui->setInsertMode();
	} 
	if (c == ':') {
		ui->setCommandMode();
	}
}


edi::UiModeCommand::UiModeCommand()
	:UiMode("COMMAND", "<Esc> -- NORMAL mode | 'q' -- quit")
{
}

edi::UiModeCommand::~UiModeCommand()
{
}

void edi::UiModeCommand::processKeyboardEvent(char c, Tui *ui)
{
	if (c == 27) {
		ui->setNormalMode();
	} 
	if (c == 'q') {
		ui->setQuitFlag(true);
	}
}


edi::UiModeInsert::UiModeInsert()
	:UiMode("INSERT", "<Esc> -- NORMAL mode")
{
}

edi::UiModeInsert::~UiModeInsert()
{
}

void edi::UiModeInsert::processKeyboardEvent(char c, Tui *ui)
{
	if (c == 27) {
		ui->setNormalMode();
	} 
}
