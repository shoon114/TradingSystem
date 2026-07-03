#pragma once
#include <string>
#include "OrderCommand.h"

class BuyOrder : public IOrderCommand {
public:
    BuyOrder(std::string stockCode, int price, int count)
        : stockCode(std::move(stockCode)), price(price), count(count) {}

    void execute(StockBrocker& brocker) override {
        brocker.buy(stockCode, price, count);
    }

private:
    std::string stockCode;
    int price;
    int count;
};
