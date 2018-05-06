#include <cstring>

#include "test.h"

// ToDo: logging

int main()
{
	printf("----%s", test_name);
	int dashes = 54 - strlen(test_name) - 4;
	while (dashes--) {
		putchar('-');
	}
	puts("");

	int ret = test_main();
	if (ret) {
		printf("....%s", test_name);
		int dots = 50 - strlen(test_name) - 4;
		while (dots--) {
			putchar('.');
		}
		printf("\033[31mFAIL\033[0m\n");
	} else {
		// this never happens btw:
		printf("\033[32mPASS\033[0m\n");
	}
	return ret;
}
