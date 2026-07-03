#pragma once
#include "stock_brocker.h"

class IOrderCommand {
public:
    virtual ~IOrderCommand() = default;
    virtual void execute(StockBrocker& brocker) = 0;
};
