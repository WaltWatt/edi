#ifndef TUI_TSIZE_H
#define TUI_TSIZE_H

namespace tui {
class TSize
{
	int _columns;
	int _rows;

public:
	TSize(int columns = 0, int rows = 0);
	virtual ~TSize();

	int columns() const;
	int rows() const;
	void setColumns(int columns);
	void setRows(int rows);
};
} // namespace tui
#endif // TUI_TSIZE_H
