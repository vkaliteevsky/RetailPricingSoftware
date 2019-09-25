/*
 * gtest_main.cpp
 *
 *  Created on: Jan 22, 2019
 *      Author: kraychik
 */


#include <stdio.h>
#include "gtest/gtest.h"
#include "CSmartException.h"

GTEST_API_ int main_test(int argc, char **argv) {
	setenv("TZ", "UTC", 1);
	srand(time(NULL));
	printf("Running main() from gtest_main.cc\n");
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

