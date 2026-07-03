#include "trading_system.h"
#include "stock_brocker.cpp"
#include "gmock/gmock.h"

using namespace testing;


int main() {
	InitGoogleMock();
	return RUN_ALL_TESTS();
}