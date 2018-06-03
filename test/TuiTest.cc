#include <gmock/gmock.h>

#include "src/ui/tui/TMainWindow.h"

using namespace tui;

TEST(TMainWindow, DefaultConstruction)
{
	TMainWindow w;
	ASSERT_TRUE(sizeof(w));
}

TEST(TMainWindow, GetNumberOfColumns)
{
	TMainWindow w;
	ASSERT_FALSE(w.cols() == 0);
	ASSERT_TRUE(w.cols() < 500);
}
