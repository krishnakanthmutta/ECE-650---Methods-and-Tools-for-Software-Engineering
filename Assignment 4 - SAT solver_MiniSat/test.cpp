/**
 * An example unit test using doctest. See documentation at
 * https://github.com/onqtam/doctest/blob/master/doc/markdown/tutorial.md
 */
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

TEST_CASE("Successful Test Example") {
    int a = 5;
    CHECK(a == 5);
}
TEST_CASE("Failing Test Examples") {
    CHECK(true == false);
}

/*
V 5
E {<1,5>,<5,2>,<1,4>,<4,5>,<4,3>,<2,4>}

V 6
E {<1,2>,<2,3>,<3,4>,<3,6>,<4,5>,<5,1>}

V 9
E {<1,2>,<2,3>,<4,5>,<6,5>,<7,3>,<2,8>,<5,3>,<2,5>,<9,3>}

V 13
E {<1,2>,<3,4>,<3,5>,<6,7>,<6,2>,<8,5>,<1,5>,<6,9>,<8,10>,<6,11>,<3,8>,<1,12>,<1,13>,<12,5>}

*/