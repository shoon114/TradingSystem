#pragma once
#include <iostream>
#include "stock_brocker.h"
#include <string>

using std::string;

class TradingSystem {
public:
	void setStockBrocker(StockBrocker* brocker) {
		stockBrocker = brocker;
	}
	void setBrocker(StockBrocker* brocker) {
		stockBrocker = brocker;
	}
	StockBrocker* selectStockBrocker(string ApiName);

	void buy(std::string stockCode, int price, int count) {
		stockBrocker->buy(stockCode, price, count);
	}
	void sell(std::string stockCode, int price, int count) {
		stockBrocker->sell(stockCode, price, count);
	}

	void buyNiceTiming(std::string stockCode, int totalAmount);
	void sellNiceTiming(std::string stockCode, int count);

private:
	StockBrocker* stockBrocker;
};