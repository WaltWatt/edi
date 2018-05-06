#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "test.h"

using namespace std;

const char *test_name = "test/functional";

/**
 * Get stdandard output of the program (as std::string).
 */
std::string exec(const char *cmd)
{
	const int buff_size = 128;
	char buffer[buff_size];
	string result = "";
	FILE *pipe = popen(cmd, "r");
	if (!pipe) {
		throw runtime_error("popen() failed!");
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

void diff(string name, string expected, string actual)
{
	if (expected.compare(actual)) {
		string s;
		s.append("Fist failure: \033[36m" + name + "\033[0m" + ":\n");
		s.append("\033[31m-" + expected + "\033[0m");
		s.append("\033[32m+" + actual + "\033[0m");
		cout << s << endl;
	}
}

static int test_no_args()
{
	const char *cmd = "./edi";
	const string expected = "I'm edi, an awesome new editor.\n";
	const string actual = exec(cmd);

        diff(__func__, expected, actual);
	return expected.compare(actual);
}

static int test_unknown_args()
{
	const char *cmd = "./edi --foo";
	std::string expected = "edi: Unknown argument \"--foo\"\n";
	std::string actual = exec(cmd);
        diff(__func__, expected, actual);
	return expected.compare(actual);

	/*
	assert(!expected.compare(actual));

	cmd = "./edi -b";
	expected.clear();
	expected.assign("edi: Unknown argument \"-b\"\n");
	actual = exec(cmd);
	assert(!expected.compare(actual));
	*/

	return 0;
}


int test_main()
{

	return     test_no_args()
		|| test_unknown_args()
	;
}
