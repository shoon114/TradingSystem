#include<iostream>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mock_stock_brocker.h"
#include "trading_system.h"

using ::testing::Return;

// StockBrocker::login
// - id, pass를 인자로 전달하면 정확히 1회 호출되어야 한다.
TEST(StockBrockerInterfaceTest, Login_CallsWithGivenIdAndPassword) {
	MockStockBrocker brocker;

	EXPECT_CALL(brocker, login("testId", "testPass")).Times(1);

	brocker.login("testId", "testPass");
}

// StockBrocker::login - 에러 핸들링
// - id가 빈 문자열이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Login_Throws_WhenIdIsEmpty) {
	MockStockBrocker brocker;
	ON_CALL(brocker, login("", "testPass"))
		.WillByDefault(::testing::Throw(std::invalid_argument("id must not be empty")));

	EXPECT_THROW(brocker.login("", "testPass"), std::invalid_argument);
}

// - password가 빈 문자열이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Login_Throws_WhenPasswordIsEmpty) {
	MockStockBrocker brocker;
	ON_CALL(brocker, login("testId", ""))
		.WillByDefault(::testing::Throw(std::invalid_argument("password must not be empty")));

	EXPECT_THROW(brocker.login("testId", ""), std::invalid_argument);
}

// - id, pass 형식은 정상이지만 외부 증권사 API가 로그인을 거부(인증 실패, 통신 오류 등)하면
//   std::runtime_error가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Login_Throws_WhenExternalApiRejectsLogin) {
	MockStockBrocker brocker;
	ON_CALL(brocker, login("testId", "wrongPass"))
		.WillByDefault(::testing::Throw(std::runtime_error("login failed: rejected by broker")));

	EXPECT_THROW(brocker.login("testId", "wrongPass"), std::runtime_error);
}

// StockBrocker::buy
// - 종목코드, 가격, 수량을 인자로 전달하면 정확히 1회 호출되어야 한다.

TEST(KiwerStockTest, Buy_DelegatesToKiwerApiWithCorrectArgs) {
	KiwerStock kiwerStock;

	std::ostringstream oss;
	auto oldCoutStreamBuf = std::cout.rdbuf(oss.rdbuf());
	std::string stockCode = "005930";
	int price = 70000;
	int count = 10;
	kiwerStock.buy(stockCode, price, count);

	std::cout.rdbuf(oldCoutStreamBuf);
	std::string expect = "";
	EXPECT_EQ(oss.str(), stockCode + " : Buy stock ( " + std::to_string(count) + " * " + std::to_string(price) + ")\n");
}
TEST(NemoStockTest, Buy_DelegatesToNemoApiWithCorrectArgs) {
	NemoStock nemoStock;

	std::ostringstream oss;
	std::streambuf* oldCoutStreamBuf = std::cout.rdbuf(oss.rdbuf());
	std::string stockCode = "005930";
	int price = 70000;
	int count = 10;
	nemoStock.buy(stockCode, price, count);

	std::cout.rdbuf(oldCoutStreamBuf);   // 검증 전에 먼저 복구
	EXPECT_EQ(oss.str(), std::string{ "[NEMO]"+ stockCode +" buy stock ( price : "+ std::to_string(price) +" ) * ( count : "+ std::to_string(count) +")\n" });
}

// StockBrocker::buy - 에러 핸들링
// - 종목코드가 빈 문자열이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenStockCodeIsEmpty) {
	MockStockBrocker brocker;
	ON_CALL(brocker, buy("", 70000, 10))
		.WillByDefault(::testing::Throw(std::invalid_argument("stockCode must not be empty")));

	EXPECT_THROW(brocker.buy("", 70000, 10), std::invalid_argument);
}

// - 가격이 0 이하이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenPriceIsNotPositive) {
	MockStockBrocker brocker;
	ON_CALL(brocker, buy("005930", 0, 10))
		.WillByDefault(::testing::Throw(std::invalid_argument("price must be positive")));

	EXPECT_THROW(brocker.buy("005930", 0, 10), std::invalid_argument);
}

// - 수량이 0 이하이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenCountIsNotPositive) {
	MockStockBrocker brocker;
	ON_CALL(brocker, buy("005930", 70000, 0))
		.WillByDefault(::testing::Throw(std::invalid_argument("count must be positive")));

	EXPECT_THROW(brocker.buy("005930", 70000, 0), std::invalid_argument);
}

// - 인자는 정상이지만 외부 증권사 API가 매수를 거부(잔고 부족, 시장 마감 등)하면
//   std::runtime_error가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenExternalApiRejectsBuy) {
	MockStockBrocker brocker;
	ON_CALL(brocker, buy("005930", 70000, 10))
		.WillByDefault(::testing::Throw(std::runtime_error("buy failed: rejected by broker")));

	EXPECT_THROW(brocker.buy("005930", 70000, 10), std::runtime_error);
}

// StockBrocker::sell
// - 종목코드, 가격, 수량을 인자로 전달하면 정확히 1회 호출되어야 한다.
TEST(StockBrockerInterfaceTest, Sell_CallsWithGivenCodePriceCount) {
	MockStockBrocker brocker;
	TradingSystem tradingSystem;
	tradingSystem.setStockBrocker(&brocker);

	EXPECT_CALL(brocker, sell("005930", 70000, 10)).Times(1);

	tradingSystem.sell("005930", 70000, 10);
}

// StockBrocker::getPrice
// - 종목코드를 인자로 전달하면, 설정된 현재가를 반환해야 한다.
TEST(StockBrockerInterfaceTest, GetPrice_ReturnsConfiguredPrice) {
	MockStockBrocker brocker;

	EXPECT_CALL(brocker, getPrice("005930")).WillOnce(Return(70000));

	int price = brocker.getPrice("005930");

	EXPECT_EQ(price, 70000);
}
