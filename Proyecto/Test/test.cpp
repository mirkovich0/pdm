/*
 * test.cpp
 *
 *  Created on: Jul 7, 2024
 *      Author: Mirko Serra
 */


#include <string>
#include <iostream>

#include "crc8.h"

int ERRORS = 0;
int TESTS = 0;

#define DIM(x) (sizeof(x)/sizeof((x)[0]))

template <class T>
void ASSERT_EQ(T value, T expected)
{
    TESTS++;
    if (value != expected) {
        ERRORS++;
        std::cerr << "Expected '" << expected << "', got '" << value << "'\n";
    }
}

int main()
{
    ////////////////////////////////////////////
    // crc8
    uint8_t temp2[] = { 0xBE, 0xEF };
    const char message[] = "Hello world!";

    ASSERT_EQ<int>(crc8(0xff, temp2, DIM(temp2)), 0x92);
    ASSERT_EQ<int>(crc8(0x00, message, DIM(message)), 0x42);

    ////////////////////////////////////////////
    if (ERRORS == 0) {
        std::cout << "All " << TESTS << " tests completed OK\n";
    } else {
        std::cerr << "Failed " << ERRORS << " of " << TESTS << " tests\n";
    }

    return ERRORS == 0? EXIT_SUCCESS : EXIT_FAILURE;
}
