#pragma once
#include <iostream>
#include "stock_brocker.h"

class TradingSystem {
public:
	void setStockBrocker(StockBrocker* brocker) {
		stockBrocker = brocker;
	}
	StockBrocker* selectStockBrocker(std::string brockerName) {
		return nullptr;
	}
	void buy(std::string stockCode, int price, int count) {
		stockBrocker->buy(stockCode, price, count);
	}
	void buyNiceTiming(std::string stockCode, int totalCount) {
	}
	void sell(std::string stockCode, int price, int count) {
		stockBrocker->sell(stockCode, price, count);
	}
	void sellNiceTiming(std::string stockCode, int count) {
	}

private:
	StockBrocker* stockBrocker;
};