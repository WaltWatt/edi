//#include "tui.h"
#include <cstdio>
#include "ui/tui/TMainWindow.h"

using namespace tui;
int main(void)
{
	/*
	Tui app;
	return app.exec();
	*/
	try {
		TMainWindow w;
		printf("window: number of columns: %d\n", w.cols());
	} catch (std::system_error &e) {
		perror(e.what());
	} catch (std::runtime_error &e) {
		perror(e.what());
	}
}
