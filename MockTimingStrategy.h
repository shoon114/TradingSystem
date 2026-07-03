#pragma once

#include "gmock/gmock.h"
#include "ITimingStrategy.h"

// ITimingStrategy interface를 검증하기 위한 Mock 구현체
class MockTimingStrategy : public ITimingStrategy {
public:
	MOCK_METHOD(bool, shouldBuy, (const std::vector<int>& priceHistory), (override));
	MOCK_METHOD(bool, shouldSell, (const std::vector<int>& priceHistory), (override));
};
