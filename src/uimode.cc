#include "uimode.h"

edi::UiModeNormal::UiModeNormal()
{
}

edi::UiModeNormal::~UiModeNormal()
{
}

void edi::UiModeNormal::processKeyboardEvent(char c, UiStd *ui)
{
	if (iscntrl(c)) {
		printf("In *normal* mode: %d\r\n", c);
	} else {
		printf("In *normal* mode: %d ('%c')\r\n", c, c);
	}
	if (c == 'i') {
		printf("Changing mode to *insert*\r\n");
		ui->setInsertMode();
	} 
	if (c == ':') {
		printf("Changing mode to *command*\r\n");
		ui->setCommandMode();
	}
}


edi::UiModeCommand::UiModeCommand()
{
}

edi::UiModeCommand::~UiModeCommand()
{
}

void edi::UiModeCommand::processKeyboardEvent(char c, UiStd *ui)
{
	if (iscntrl(c)) {
		printf("In *command* mode: %d\r\n", c);
	} else {
		printf("In *command* mode: %d ('%c')\r\n", c, c);
	}
	if (c == 27) {
		printf("Changing mode to *normal*\r\n");
		ui->setNormalMode();
	} 
}


edi::UiModeInsert::UiModeInsert()
{
}

edi::UiModeInsert::~UiModeInsert()
{
}

void edi::UiModeInsert::processKeyboardEvent(char c, UiStd *ui)
{
	if (iscntrl(c)) {
		printf("In *insert* mode: %d\r\n", c);
	} else {
		printf("In *insert* mode: %d ('%c')\r\n", c, c);
	}
	if (c == 27) {
		printf("Changing mode to *normal*\r\n");
		ui->setNormalMode();
	} 
}
