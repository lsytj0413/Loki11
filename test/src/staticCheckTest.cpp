#include "staticCheckTest.h"

#include <Loki11/static_check.hpp>


TEST_F(StaticCheckTest, test)
{
    STATIC_CHECK(sizeof(char)<sizeof(int), char_less_than_int);
}


// TEST_F(StaticCheckTest, testCompileFail)
// {
//     STATIC_CHECK(sizeof(char)>=sizeof(int), char_less_than_int);
// }
