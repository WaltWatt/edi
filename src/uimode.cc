#include "uimode.h"

edi::UiModeNormal::UiModeNormal()
{
}

edi::UiModeNormal::~UiModeNormal()
{
}

void edi::UiModeNormal::processKeyboardEvent(char c, UiStd *ui)
{
	printf("In 'normal' mode: ");
	if (c == 'i') {
		ui->setInsertMode();
	} 
	if (c == ':') {
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
	printf("In 'command' mode: ");
	if (c == 27) {
		ui->setNormalMode();
	} 
	/*
	if (c == 'q') {
		//ui->quit();
	}
	*/
}


edi::UiModeInsert::UiModeInsert()
{
}

edi::UiModeInsert::~UiModeInsert()
{
}

void edi::UiModeInsert::processKeyboardEvent(char c, UiStd *ui)
{
	printf("In 'insert' mode: ");
	if (c == 27) {
		ui->setNormalMode();
	} 
}
