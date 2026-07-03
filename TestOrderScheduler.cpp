#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mock_stock_brocker.h"
#include "OrderCommand.h"
#include <memory>

using ::testing::Return;

// IOrderCommand::BuyOrder
// - execute(brocker) 호출 시 brocker.buy(종목코드, 가격, 수량)이 호출되어야 한다.
TEST(OrderCommandTest, BuyOrder_Execute_DelegatesToBrockerBuy) {
	MockStockBrocker brocker;
	BuyOrder order("005930", 70000, 10);

	EXPECT_CALL(brocker, buy("005930", 70000, 10)).Times(1);

	order.execute(brocker);
}

// IOrderCommand::SellOrder
// - execute(brocker) 호출 시 brocker.sell(종목코드, 가격, 수량)이 호출되어야 한다.
TEST(OrderCommandTest, SellOrder_Execute_DelegatesToBrockerSell) {
	MockStockBrocker brocker;
	SellOrder order("005930", 70000, 10);

	EXPECT_CALL(brocker, sell("005930", 70000, 10)).Times(1);

	order.execute(brocker);
}
