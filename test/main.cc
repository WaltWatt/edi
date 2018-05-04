#include <cstring>

#include "test.h"

// ToDo: logging

int main()
{
	printf("%s", test_name);
	int dots = 50 - strlen(test_name);
	while (dots--) {
		putchar('.');
	}

	int ret = test_main();
	if (ret) {
		printf("\033[31mFAIL\033[0m\n");
	} else {
		printf("\033[32mPASS\033[0m\n");
	}
	return ret;
}
