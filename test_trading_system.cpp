#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "trading_system.h"

// TradingSystem::selectStockBrocker
// - 지원하는 증권사(예: "KIWER", "NEMO")를 선택하면 예외 없이 동작해야 한다.
// - 선택 파라미터의 구체적 타입, 내부 검증 로직은 기능 구현 담당자가 자유롭게 결정한다.
TEST(TradingSystemTest, SelectStockBrocker_DoesNotThrow_ForSupportedBrockers) {
	TradingSystem tradingSystem;

	EXPECT_NO_THROW(tradingSystem.selectStockBrocker("KIWER"));
	EXPECT_NO_THROW(tradingSystem.selectStockBrocker("NEMO"));
}
