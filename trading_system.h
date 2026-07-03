#pragma once
#include <iostream>
#include <string>
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
	void buyNiceTiming(std::string stockCode, int totalAmount) {
		if (!stockBrocker) return;

		int p1 = stockBrocker->getPrice(stockCode);
		int p2 = stockBrocker->getPrice(stockCode);
		int p3 = stockBrocker->getPrice(stockCode);

		if (p1 < p2 && p2 < p3) {
			int quantity = 0;
			if (p3 > 0) quantity = totalAmount / p3;
			if (quantity > 0) stockBrocker->buy(stockCode, p3, quantity);
		}
	}
	void sell(std::string stockCode, int price, int count) {
		stockBrocker->sell(stockCode, price, count);
	}
	void sellNiceTiming(std::string stockCode, int count) {
		if (!stockBrocker) return;

		int p1 = stockBrocker->getPrice(stockCode);
		int p2 = stockBrocker->getPrice(stockCode);
		int p3 = stockBrocker->getPrice(stockCode);

		if (p1 > p2 && p2 > p3) {
			stockBrocker->sell(stockCode, p3, count);
		}
	}

private:
	StockBrocker* stockBrocker;
};