#pragma once

#include "gmock/gmock.h"
#include "stock_brocker.h"

// StockBrocker interface를 검증하기 위한 Mock 구현체
// 실제 Kiwer/Nemo API는 사용하지 않고, interface 계약만 검증한다.
class MockStockBrocker : public StockBrocker {
public:
	MOCK_METHOD(void, login, (std::string id, std::string pass), (override));
	MOCK_METHOD(void, buy, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(void, sell, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(int, getPrice, (std::string stockCode), (override));
};
