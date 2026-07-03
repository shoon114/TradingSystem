#pragma once
#include <iostream>
#include <string>
#include <stdexcept>
#include "stock_brocker.h"

using std::string;

class TradingSystem {
public:
	void setStockBrocker(StockBrocker* brocker) {
		stockBrocker = brocker;
	}
	void setBrocker(StockBrocker* brocker) {
		stockBrocker = brocker;
	}
	void selectStockBrocker(string ApiName) {
		if (!(ApiName == "KIWER" || ApiName == "NEMO")) {
			throw std::invalid_argument("지원하지 않는 증권사입니다: " + ApiName);
		}
	}

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