#include<iostream>
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "mock_stock_brocker.h"
#include "trading_system.h"
#include "stock_brocker.h"
#include "test_stock_brocker.h"
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

class BuyFixture : public testing::Test {
public:
	void run(StockBrocker& stockBrocker, std::string stockCode, int price, int count,
		const std::string& expected) {

		std::ostringstream oss;
		std::streambuf* oldCoutStreamBuf = std::cout.rdbuf(oss.rdbuf());

		stockBrocker.buy(stockCode, price, count);

		std::cout.rdbuf(oldCoutStreamBuf);

		EXPECT_EQ(oss.str(), expected);
	}
	std::string getKiwerBuyString(const std::string& stockCode, int price, int count)
	{
		return stockCode + " : Buy stock ( " + std::to_string(price) + " * " + std::to_string(count) + ")\n";
	}
	std::string getNemoBuyString(const std::string& stockCode, int price, int count)
	{
		return "[NEMO]" + stockCode + " buy stock ( price : " + std::to_string(price) + " ) * ( count : " + std::to_string(count) + ")\n";
	}
};
TEST_F(BuyFixture, Buy_DelegatesToKiwerApiWithCorrectArgs) {
	KiwerStock kiwerStock;
	run(kiwerStock, "005930", 70000, 10, getKiwerBuyString("005930", 70000, 10));
}

TEST_F(BuyFixture, Buy_DelegatesToNemoApiWithCorrectArgs) {
	NemoStock nemoStock;
	run(nemoStock, "005930", 70000, 10, getNemoBuyString("005930", 70000, 10));
}

// StockBrocker::buy - 에러 핸들링
// - 종목코드가 빈 문자열이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenStockCodeIsEmpty) {
	MockStockBrocker brocker;

	EXPECT_CALL(brocker, doBuy(::testing::_, ::testing::_, ::testing::_)).Times(0);

	EXPECT_THROW(brocker.buy("", 70000, 10), std::invalid_argument);
}

// - 가격이 0 이하이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenPriceIsNotPositive) {
	MockStockBrocker brocker;
	EXPECT_CALL(brocker, doBuy(::testing::_, ::testing::_, ::testing::_)).Times(0);

	EXPECT_THROW(brocker.buy("005930", 0, 10), std::invalid_argument);
}

// - 수량이 0 이하이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenCountIsNotPositive) {
	MockStockBrocker brocker;
	EXPECT_CALL(brocker, doBuy(::testing::_, ::testing::_, ::testing::_)).Times(0);

	EXPECT_THROW(brocker.buy("005930", 70000, 0), std::invalid_argument);
}

// - 인자는 정상이지만 외부 증권사 API가 매수를 거부(잔고 부족, 시장 마감 등)하면
//   std::runtime_error가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Buy_Throws_WhenExternalApiRejectsBuy) {
	MockStockBrocker brocker;
	EXPECT_CALL(brocker, doBuy("005930", 70000, 10))
		.WillOnce(::testing::Throw(std::runtime_error("insufficient balance")));

	try {
		brocker.buy("005930", 70000, 10);
		FAIL();
	}
	catch (const std::runtime_error& e) {
		EXPECT_EQ(std::string(e.what()), "buy failed: rejected by broker");
	}
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

// StockBrocker::sell - 에러 핸들링
// - 종목코드가 빈 문자열이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Sell_Throws_WhenStockCodeIsEmpty) {
	MockStockBrocker brocker;
	ON_CALL(brocker, sell("", 70000, 10))
		.WillByDefault(::testing::Throw(std::invalid_argument("stockCode must not be empty")));

	EXPECT_THROW(brocker.sell("", 70000, 10), std::invalid_argument);
}

// - 가격이 0 이하이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Sell_Throws_WhenPriceIsNotPositive) {
	MockStockBrocker brocker;
	ON_CALL(brocker, sell("005930", 0, 10))
		.WillByDefault(::testing::Throw(std::invalid_argument("price must be positive")));

	EXPECT_THROW(brocker.sell("005930", 0, 10), std::invalid_argument);
}

// - 수량이 0 이하이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Sell_Throws_WhenCountIsNotPositive) {
	MockStockBrocker brocker;
	ON_CALL(brocker, sell("005930", 70000, 0))
		.WillByDefault(::testing::Throw(std::invalid_argument("count must be positive")));

	EXPECT_THROW(brocker.sell("005930", 70000, 0), std::invalid_argument);
}

// - 인자는 정상이지만 외부 증권사 API가 매도를 거부(보유수량 부족, 시장 마감 등)하면
//   std::runtime_error가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, Sell_Throws_WhenExternalApiRejectsSell) {
	MockStockBrocker brocker;
	ON_CALL(brocker, sell("005930", 70000, 10))
		.WillByDefault(::testing::Throw(std::runtime_error("sell failed: rejected by broker")));

	EXPECT_THROW(brocker.sell("005930", 70000, 10), std::runtime_error);
}

// StockBrocker::getPrice
// - 종목코드를 인자로 전달하면, 설정된 현재가를 반환해야 한다.
TEST(StockBrockerInterfaceTest, GetPrice_ReturnsConfiguredPrice) {
	MockStockBrocker brocker;

	EXPECT_CALL(brocker, getPrice("005930")).WillOnce(Return(70000));

	int price = brocker.getPrice("005930");

	EXPECT_EQ(price, 70000);
}

// StockBrocker::getPrice - 에러 핸들링
// - 종목코드가 빈 문자열이면 std::invalid_argument가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, GetPrice_Throws_WhenStockCodeIsEmpty) {
	MockStockBrocker brocker;
	ON_CALL(brocker, getPrice(""))
		.WillByDefault(::testing::Throw(std::invalid_argument("stockCode must not be empty")));

	EXPECT_THROW(brocker.getPrice(""), std::invalid_argument);
}

// - 종목코드는 정상이지만 외부 증권사 API가 시세 조회를 거부(존재하지 않는 종목, 통신 오류 등)하면
//   std::runtime_error가 전파되어야 한다.
TEST(StockBrockerInterfaceTest, GetPrice_Throws_WhenExternalApiRejectsGetPrice) {
	MockStockBrocker brocker;
	ON_CALL(brocker, getPrice("005930"))
		.WillByDefault(::testing::Throw(std::runtime_error("getPrice failed: rejected by broker")));

	EXPECT_THROW(brocker.getPrice("005930"), std::runtime_error);
}

TEST(KiwerStockTest, GetPriceInRange)
{
	KiwerStock brocker;

	int price = brocker.getPrice("005930");

	EXPECT_THAT(price, testing::AllOf(testing::Ge(5000), testing::Lt(6000)));
}

TEST(NemoStockTest, GetPriceInRange)
{
	NemoStock brocker;

	int price = brocker.getPrice("005930");

	EXPECT_THAT(price, testing::AllOf(testing::Ge(5000), testing::Lt(6000)));
}