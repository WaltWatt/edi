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
	ASSERT_FALSE(w.columns() == 0);
	ASSERT_TRUE(w.columns() < 500);
}

TEST(TMainWindow, GetNumberOfRows)
{
	TMainWindow w;
	ASSERT_FALSE(w.rows() == 0);
	ASSERT_TRUE(w.rows() < 140);
}
