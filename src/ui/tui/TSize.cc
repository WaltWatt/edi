#include "TSize.h"

using namespace tui;
tui::TSize::TSize(int columns, int rows)
	: _columns(columns),
	  _rows(rows)
{
}

tui::TSize::~TSize()
{
}

int tui::TSize::columns() const
{
	return _columns;
}

int tui::TSize::rows() const
{
	return _rows;
}

void tui::TSize::setColumns(int columns)
{
	_columns = columns;
}

void tui::TSize::setRows(int rows)
{
	_rows = rows;
}
