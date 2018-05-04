#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "test.h"

const char *test_name = "test/functional";

std::string exec(const char *cmd)
{
	const int buff_size = 128;
	char buffer[buff_size];
	std::string result = "";
	FILE *pipe = popen(cmd, "r");
	if (!pipe) {
		throw std:: runtime_error("popen() failed!");
	}
	try {
		while (!feof(pipe)) {
			if (fgets(buffer, buff_size, pipe) != NULL) {
				result += buffer;
			}
		}
	} catch (...) {
		pclose(pipe);
		throw;
	}
	pclose(pipe);
	return result;
}

int test_main()
{
	const char *cmd = "./edi";
	const std::string expected = "I'm edi, an awesome new editor.\n";
	const std::string actual = exec(cmd);

	return expected.compare(actual);
}
