#include <iostream>
#include <string>
#include <vector>
#include "HelperFuncs.h"

static void Check(bool cond, const char* msg, int& fails)
{
    if (!cond) {
        std::cerr << "FAIL: " << msg << "\n";
        ++fails;
    }
}

int main()
{
    int fails = 0;

    // isValidPassword tests
    Check(isValidPassword("abc12345"), "valid password should pass", fails);
    Check(!isValidPassword("abcdefg"), "no digit should fail", fails);
    Check(!isValidPassword("12345678"), "no letter should fail", fails);

    std::vector<std::string> v;

    // vectorToString tests
    v = {"x", "y"};
    auto joined = vectorToString(v);
    Check(joined == std::string("x|y|"), "vectorToString should join with |", fails);

    if (fails == 0) {
        std::cout << "All tests passed\n";
        return 0;
    }
    std::cerr << fails << " test(s) failed\n";
    return 1;
}
