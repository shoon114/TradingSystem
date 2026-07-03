#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "stock_brocker.h"

using ::testing::Return;

// StockBrocker interface를 검증하기 위한 Mock 구현체
// 실제 Kiwer/Nemo API는 사용하지 않고, interface 계약만 검증한다.
class MockStockBrocker : public StockBrocker {
public:
	MOCK_METHOD(void, login, (std::string id, std::string pass), (override));
	MOCK_METHOD(void, buy, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(void, sell, (std::string stockCode, int price, int count), (override));
	MOCK_METHOD(int, getPrice, (std::string stockCode), (override));
};

// StockBrocker::login
// - id, pass를 인자로 전달하면 정확히 1회 호출되어야 한다.
TEST(StockBrockerInterfaceTest, Login_CallsWithGivenIdAndPassword) {
	MockStockBrocker brocker;

	EXPECT_CALL(brocker, login("testId", "testPass")).Times(1);

	brocker.login("testId", "testPass");
}
