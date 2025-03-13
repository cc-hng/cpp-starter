
#include "stdafx.h"
#include <fmt/core.h>

int main() {
    auto _defer = gsl::finally([] { fmt::print("exit.\n"); });
    fmt::print("hello,world!\n");
    g_asp.set_timeout(1000, [] { fmt::print("timeout 1s\n"); });
    g_asp.run(1, false);
    return 0;
}
