#pragma once
#include <string>

class StockBrocker {
public:
    // 가상 소멸자는 상속받는 클래스에서 필수입니다.
    virtual ~StockBrocker() = default;

    // 순수 가상 함수들 (자식 클래스에서 반드시 구현해야 함)
    virtual void login(std::string id, std::string pass) = 0;
    virtual void buy(std::string stockCode, int price, int count) = 0;
    virtual void sell(std::string stockCode, int price, int count) = 0;
    virtual int getPrice(std::string stockCode) = 0;
};
