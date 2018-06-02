#ifndef TUI_TSIZE_H
#define TUI_TSIZE_H

namespace tui {
class TSize
{
	int _width;
	int _height;

public:
	TSize(int width = 0, int height = 0);

	int width() const;
	int height() const;
	void setWidth(int width);
	void setHeight(int height);
};
} // namespace tui
#endif // TUI_TSIZE_H
