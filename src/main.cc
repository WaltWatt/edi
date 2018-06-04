//#include "tui.h"
#include <iostream>
#include "ui/tui/TMainWindow.h"

using namespace std;
using namespace tui;
int main(void)
{
	/*
	Tui app;
	return app.exec();
	*/
	try {
		TMainWindow w;

		cout << "main window size: " << w.columns() << " columns and ";
		cout << w.rows() << " rows." << endl;
	} catch (std::system_error &e) {
		cerr << e.what() << endl;
	} catch (std::runtime_error &e) {
		cerr << e.what() << endl;
	}
}
