#include <string>
#include <iostream>
#include "stock_brocker.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"


// 1. Kiwer 증권사 구현
class KiwerStock : public StockBrocker {
private:
	KiwerAPI KiwerApi; // Kiwer API 객체
public:
    void login(std::string id, std::string pass) override {
        // Kiwer의 실제 API 호출
        KiwerApi.login(id, pass); // 어댑터가 내부 API 호출
    }

    void buy(std::string stockCode, int price, int count) override {
        KiwerApi.buy(stockCode, price, count);

    }

    void sell(std::string stockCode, int price, int count) override {
    }

    int getPrice(std::string stockCode) override {
        return KiwerApi.currentPrice(stockCode);
    }
};

// 2. Nemo 증권사 구현
class NemoStock : public StockBrocker {
private:
    NemoAPI NemoApi; // NemoAPI 객체를 내부 멤버로 가짐
    const int MIN_SLEEP_TIME = 1;
public:
    void login(std::string id, std::string pass) override {
        NemoApi.certification(id, pass);
    }

    void buy(std::string stockCode, int price, int count) override {
        NemoApi.purchasingStock(stockCode, price, count);
    }

    void sell(std::string stockCode, int price, int count) override {
    }

    int getPrice(std::string stockCode) override {
        return NemoApi.getMarketPrice(stockCode, MIN_SLEEP_TIME);
    }
};