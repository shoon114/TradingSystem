#include <string>
#include <iostream>
#include "stock_brocker.h"
#include "kiwer_api.cpp"
#include "nemo_api.cpp"

// 1. Kiwer 증권사 구현
void KiwerStock::login(std::string id, std::string pass) {
    // Kiwer의 실제 API 호출
    KiwerApi.login(id, pass); // 어댑터가 내부 API 호출
}
void KiwerStock::buy(std::string stockCode, int price, int count) {
    KiwerApi.buy(stockCode, price, count);

}
void KiwerStock::sell(std::string stockCode, int price, int count) {
}
int KiwerStock::getPrice(std::string stockCode) {
    return KiwerApi.currentPrice(stockCode);
}

// 2. Nemo 증권사 구현
void NemoStock::login(std::string id, std::string pass) {
    NemoApi.certification(id, pass);
}
void NemoStock::buy(std::string stockCode, int price, int count) {
    NemoApi.purchasingStock(stockCode, price, count);
}
void NemoStock::sell(std::string stockCode, int price, int count) {
}
int NemoStock::getPrice(std::string stockCode) {
    return NemoApi.getMarketPrice(stockCode, MIN_SLEEP_TIME);
}