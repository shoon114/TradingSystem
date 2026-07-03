#pragma once
#include "stock_brocker.h"
#include <string>

using std::string;

class TradingSystem {
public:
	StockBrocker* selectStockBrocker(string ApiName);
private:
	StockBrocker* stockBrocker;
};