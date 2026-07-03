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

	void buy(std::string stockCode, int price, int count) {
		stockBrocker->buy(stockCode, price, count);
	}
	StockBrocker* selectStockBrocker(std::string brockerName) {
    	if (!(brockerName == "KIWER" || brockerName == "NEMO")) {
			throw std::invalid_argument("제공하지 않는 증권사 입니다. " + brockerName);
		}
		return stockBrocker;
	}
	void buy(std::string stockCode, int price, int count) {
		stockBrocker->buy(stockCode, price, count);
	}
	void buyNiceTiming(std::string stockCode, int totalCount) {
	}
	void sell(std::string stockCode, int price, int count) {
		stockBrocker->sell(stockCode, price, count);
	}
	
	void buyNiceTiming(std::string stockCode, int totalAmount);
	void sellNiceTiming(std::string stockCode, int count);


private:
	StockBrocker* stockBrocker;
};