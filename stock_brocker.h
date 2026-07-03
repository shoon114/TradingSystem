#pragma once
#include <string>
#include <stdexcept>
// 추상 기본 클래스 (인터페이스)
class StockBrocker {
public:
    // 가상 소멸자는 상속받는 클래스에서 필수입니다.
    virtual ~StockBrocker() = default;

    // 순수 가상 함수들 (자식 클래스에서 반드시 구현해야 함)
    virtual void login(std::string id, std::string pass) = 0;
    void buy(std::string stockCode, int price, int count) {
        if (stockCode.empty()) {
            throw std::invalid_argument("stockCode must not be empty");
        }
        if (price <= 0) {
            throw std::invalid_argument("price must be positive");
        }
        if (count <= 0) {
            throw std::invalid_argument("count must be positive");
        }
        try {
            doBuy(stockCode, price, count);
        }
        catch (const std::exception&) {
            throw std::runtime_error("buy failed: rejected by broker");
        }
    }
    virtual void sell(std::string stockCode, int price, int count) = 0;
    virtual int getPrice(std::string stockCode) = 0;
protected:
    virtual void doBuy(std::string stockCode, int price, int count) = 0;
};

