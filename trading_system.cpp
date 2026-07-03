#include "trading_system.h"
#include "stock_brocker.cpp"
#include "gmock/gmock.h"
#include <stdexcept>

using namespace testing;

StockBrocker* TradingSystem::selectStockBrocker(string ApiName) {
    if (ApiName == "KIWER") {
        stockBrocker = new KiwerStock();
    }
    else if (ApiName == "NEMO") {
        stockBrocker = new NemoStock();
    }
    else {
        throw std::invalid_argument("지원하지 않는 증권사입니다: " + ApiName);
    }
    return stockBrocker;
}

int main() {
	InitGoogleMock();
	return RUN_ALL_TESTS();
}