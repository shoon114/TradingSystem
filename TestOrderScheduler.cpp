#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mock_stock_brocker.h"
#include "MockLogger.h"
#include "OrderCommand.h"
#include "OrderScheduler.h"
#include <memory>
#include <chrono>

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

// OrderScheduler::scheduleOrder / processDueOrders
// - 예약된 실행시각(executeTime)에 도달하면(now >= executeTime), 큐에 저장된 주문을
//   실행하고 로그를 남겨야 한다.
TEST(OrderSchedulerTest, ProcessDueOrders_ExecutesAndLogsOrder_WhenTimeReached) {
	MockStockBrocker brocker;
	MockLogger logger;
	OrderScheduler scheduler(&brocker, &logger);

	auto executeTime = std::chrono::system_clock::now();
	auto order = std::make_shared<BuyOrder>("005930", 70000, 10);
	scheduler.scheduleOrder(order, executeTime);

	EXPECT_CALL(brocker, buy("005930", 70000, 10)).Times(1);
	EXPECT_CALL(logger, log(::testing::_)).Times(1);

	scheduler.processDueOrders(executeTime);
}

// - 예약된 실행시각에 아직 도달하지 않았다면(now < executeTime), 주문을 실행하지 않고
//   로그도 남기지 않아야 한다.
TEST(OrderSchedulerTest, ProcessDueOrders_DoesNotExecuteOrder_BeforeScheduledTime) {
	MockStockBrocker brocker;
	MockLogger logger;
	OrderScheduler scheduler(&brocker, &logger);

	auto now = std::chrono::system_clock::now();
	auto executeTime = now + std::chrono::hours(1);
	auto order = std::make_shared<BuyOrder>("005930", 70000, 10);
	scheduler.scheduleOrder(order, executeTime);

	EXPECT_CALL(brocker, buy(::testing::_, ::testing::_, ::testing::_)).Times(0);
	EXPECT_CALL(logger, log(::testing::_)).Times(0);

	scheduler.processDueOrders(now);
}
