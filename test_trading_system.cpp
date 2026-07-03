#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "trading_system.h"
#include "mock_stock_brocker.h"
#include "OrderCommand.h"
#include <memory>
#include <chrono>

using namespace testing;

// TradingSystem::selectStockBrocker
// - 지원하는 증권사(예: "KIWER", "NEMO")를 선택하면 예외 없이 동작해야 한다.
// - 선택 파라미터의 구체적 타입, 내부 검증 로직은 기능 구현 담당자가 자유롭게 결정한다.
TEST(TradingSystemTest, SelectStockBrocker_DoesNotThrow_ForSupportedBrockers) {
	TradingSystem tradingSystem;

	EXPECT_NO_THROW(tradingSystem.selectStockBrocker("KIWER"));
	EXPECT_NO_THROW(tradingSystem.selectStockBrocker("NEMO"));
}

// TradingSystem::buyNiceTiming
// - 200ms 주기로 3회 가격을 읽어 3회 연속 상승 추세이면,
//   총 금액으로 살 수 있는 최대 수량을 마지막 가격으로 매수해야 한다.
TEST(TradingSystemTest, BuyNiceTiming_BuysMaxQuantityAtLastPrice_WhenPriceRisesThreeTimes) {
	MockStockBrocker mockBrocker;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&mockBrocker);

	const std::string stockCode = "005930";
	const int totalAmount = 100000;

	EXPECT_CALL(mockBrocker, getPrice(stockCode))
		.Times(3)
		.WillOnce(Return(5000))
		.WillOnce(Return(5100))
		.WillOnce(Return(5200));

	// quantity = totalAmount / lastPrice = 100000 / 5200 = 19
	EXPECT_CALL(mockBrocker, doBuy(stockCode, 5200, 19)).Times(1);

	tradingSystem.buyNiceTiming(stockCode, totalAmount);
}

// TradingSystem::sellNiceTiming
// - 200ms 주기로 3회 가격을 읽어 3회 연속 하락 추세이면,
//   사용자가 설정한 수량을 마지막 가격으로 모두 매도해야 한다.
TEST(TradingSystemTest, SellNiceTiming_SellsGivenQuantityAtLastPrice_WhenPriceFallsThreeTimes) {
	MockStockBrocker mockBrocker;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&mockBrocker);

	const std::string stockCode = "005930";
	const int quantity = 10;

	EXPECT_CALL(mockBrocker, getPrice(stockCode))
		.Times(3)
		.WillOnce(Return(5200))
		.WillOnce(Return(5100))
		.WillOnce(Return(5000));

	EXPECT_CALL(mockBrocker, sell(stockCode, 5000, quantity)).Times(1);

	tradingSystem.sellNiceTiming(stockCode, quantity);
}

// TradingSystem::ScheduleOrder
// - 주문(BuyOrder/SellOrder)과 실행시각을 전달하면 예외 없이 예약되어야 한다.
// - 실제 큐 관리, 실행, 로깅은 내부 OrderScheduler가 담당하며,
//   TradingSystem이 이를 어떻게 소유/연결하는지는 구현 담당자가 자유롭게 결정한다.
TEST(TradingSystemTest, ScheduleOrder_DoesNotThrow_WhenGivenValidOrderAndTime) {
	TradingSystem tradingSystem;
	auto order = std::make_shared<BuyOrder>("005930", 70000, 10);
	auto executeTime = std::chrono::system_clock::now() + std::chrono::seconds(1);

	EXPECT_NO_THROW(tradingSystem.ScheduleOrder(order, executeTime));
}
