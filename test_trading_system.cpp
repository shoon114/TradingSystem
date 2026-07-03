#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "trading_system.h"
#include "mock_stock_brocker.h"
#include "OrderCommand.h"
#include "MockTimingStrategy.h"
#include <memory>
#include <chrono>
#include <vector>

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

// TradingSystem::buyNiceTiming (전략 연동)
// - setStrategy()로 주입된 ITimingStrategy의 shouldBuy(priceHistory)가 true를 반환하면,
//   3회 조회한 가격 이력을 기반으로 마지막 가격으로 최대 수량을 매수해야 한다.
TEST(TradingSystemStrategyTest, BuyNiceTiming_Buys_WhenStrategyShouldBuyReturnsTrue) {
	MockStockBrocker mockBrocker;
	MockTimingStrategy mockStrategy;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&mockBrocker);
	tradingSystem.setStrategy(&mockStrategy);

	const std::string stockCode = "005930";
	const int totalAmount = 100000;

	EXPECT_CALL(mockBrocker, getPrice(stockCode))
		.Times(3)
		.WillOnce(Return(5000))
		.WillOnce(Return(5100))
		.WillOnce(Return(5200));

	EXPECT_CALL(mockStrategy, shouldBuy(std::vector<int>{5000, 5100, 5200}))
		.WillOnce(Return(true));

	// quantity = totalAmount / lastPrice = 100000 / 5200 = 19
	EXPECT_CALL(mockBrocker, doBuy(stockCode, 5200, 19)).Times(1);

	tradingSystem.buyNiceTiming(stockCode, totalAmount);
}

// - shouldBuy(priceHistory)가 false를 반환하면 매수하지 않아야 한다.
TEST(TradingSystemStrategyTest, BuyNiceTiming_DoesNotBuy_WhenStrategyShouldBuyReturnsFalse) {
	MockStockBrocker mockBrocker;
	MockTimingStrategy mockStrategy;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&mockBrocker);
	tradingSystem.setStrategy(&mockStrategy);

	const std::string stockCode = "005930";
	const int totalAmount = 100000;

	EXPECT_CALL(mockBrocker, getPrice(stockCode))
		.Times(3)
		.WillOnce(Return(5200))
		.WillOnce(Return(5100))
		.WillOnce(Return(5000));

	EXPECT_CALL(mockStrategy, shouldBuy(std::vector<int>{5200, 5100, 5000}))
		.WillOnce(Return(false));

	EXPECT_CALL(mockBrocker, doBuy(_, _, _)).Times(0);

	tradingSystem.buyNiceTiming(stockCode, totalAmount);
}

// TradingSystem::sellNiceTiming (전략 연동)
// - setStrategy()로 주입된 ITimingStrategy의 shouldSell(priceHistory)가 true를 반환하면,
//   사용자가 설정한 수량을 마지막 가격으로 모두 매도해야 한다.
TEST(TradingSystemStrategyTest, SellNiceTiming_Sells_WhenStrategyShouldSellReturnsTrue) {
	MockStockBrocker mockBrocker;
	MockTimingStrategy mockStrategy;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&mockBrocker);
	tradingSystem.setStrategy(&mockStrategy);

	const std::string stockCode = "005930";
	const int quantity = 10;

	EXPECT_CALL(mockBrocker, getPrice(stockCode))
		.Times(3)
		.WillOnce(Return(5200))
		.WillOnce(Return(5100))
		.WillOnce(Return(5000));

	EXPECT_CALL(mockStrategy, shouldSell(std::vector<int>{5200, 5100, 5000}))
		.WillOnce(Return(true));

	EXPECT_CALL(mockBrocker, sell(stockCode, 5000, quantity)).Times(1);

	tradingSystem.sellNiceTiming(stockCode, quantity);
}

// - shouldSell(priceHistory)가 false를 반환하면 매도하지 않아야 한다.
TEST(TradingSystemStrategyTest, SellNiceTiming_DoesNotSell_WhenStrategyShouldSellReturnsFalse) {
	MockStockBrocker mockBrocker;
	MockTimingStrategy mockStrategy;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&mockBrocker);
	tradingSystem.setStrategy(&mockStrategy);

	const std::string stockCode = "005930";
	const int quantity = 10;

	EXPECT_CALL(mockBrocker, getPrice(stockCode))
		.Times(3)
		.WillOnce(Return(5000))
		.WillOnce(Return(5100))
		.WillOnce(Return(5200));

	EXPECT_CALL(mockStrategy, shouldSell(std::vector<int>{5000, 5100, 5200}))
		.WillOnce(Return(false));

	EXPECT_CALL(mockBrocker, sell(_, _, _)).Times(0);

	tradingSystem.sellNiceTiming(stockCode, quantity);
}

// TradingSystem::setStrategy
// - setStrategy()는 런타임에 언제든지 다른 전략으로 교체할 수 있어야 한다.
//   교체 후 buyNiceTiming을 호출하면, 이전 전략이 아니라 새로 설정된
//   전략의 shouldBuy(priceHistory) 판단을 따라야 한다.
TEST(TradingSystemStrategyTest, SetStrategy_ReplacesStrategyAtRuntime) {
	MockStockBrocker mockBrocker;
	MockTimingStrategy oldStrategy;
	MockTimingStrategy newStrategy;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&mockBrocker);

	const std::string stockCode = "005930";
	const int totalAmount = 100000;

	tradingSystem.setStrategy(&oldStrategy);
	tradingSystem.setStrategy(&newStrategy);

	EXPECT_CALL(mockBrocker, getPrice(stockCode))
		.Times(3)
		.WillOnce(Return(5000))
		.WillOnce(Return(5100))
		.WillOnce(Return(5200));

	EXPECT_CALL(oldStrategy, shouldBuy(_)).Times(0);
	EXPECT_CALL(newStrategy, shouldBuy(std::vector<int>{5000, 5100, 5200}))
		.WillOnce(Return(true));
	EXPECT_CALL(mockBrocker, doBuy(stockCode, 5200, 19)).Times(1);

	tradingSystem.buyNiceTiming(stockCode, totalAmount);
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
