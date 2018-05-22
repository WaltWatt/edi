#ifndef CEXCEPTION_H
#define CEXCEPTION_H

namespace edi {
class CException
{
	const char *const _msg;

public:
	CException(const char *const msg = 0) : _msg(msg)
	{}
	const char *what() {
		return _msg;
	}
};
}

#endif // CEXCEPTION_H
