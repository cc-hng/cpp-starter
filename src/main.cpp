
#include "stdafx.h"
#include <stdio.h>
#include <strings.hpp>

int main() {
    printf("hello,%s\n", ss::str_tolower("WORLD").c_str());
    return 0;
}
