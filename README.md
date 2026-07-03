# TradingSystem

## 1. System Summary

Nemo증권과 Kiwer증권, 두 증권사가 각각 제공하는 주식거래 API의 인터페이스를 통합하여 사용하는 **Trading System**입니다.

두 증권사 API는 로그인, 매수, 매도, 현재가 조회 등 유사한 기능을 제공하지만 함수명, 파라미터 순서, 반환 방식 등이 서로 다릅니다. 본 시스템은 `StockBrocker` 인터페이스를 두고 각 증권사 API를 어댑터로 감싸, 향후 새로운 증권사가 추가되더라도 기존 코드를 변경하지 않고 확장할 수 있도록 설계합니다. (OCP: Open-Closed Principle)

또한 단순 매수/매도 기능 외에, 200ms 주기로 가격을 조회하여 추세를 판단한 뒤 매매 타이밍을 결정하는 핵심 기능(`buyNiceTiming`, `sellNiceTiming`)을 제공합니다.

본 프로젝트는 TDD(Test-Driven Development) 방법론으로 진행되며, GoogleTest/GoogleMock 기반의 단위 테스트를 통해 기능을 검증합니다.

## 2. System Requirement

### 2.1 제공되는 증권사 API

#### KiwerStock

| 함수 | 설명 |
| --- | --- |
| `login(ID, PASS)` | 로그인 |
| `buy(종목코드, 수량, 가격)` | 매수 |
| `sell(종목코드, 수량, 가격)` | 매도 |
| `currentPrice(종목코드)` | 현재가 조회 |

#### NemoStock

| 함수 | 설명 |
| --- | --- |
| `certification(ID, PASS)` | 로그인 |
| `purchasingStock(종목코드, 가격, 수량)` | 매수 |
| `sellingStock(종목코드, 가격, 수량)` | 매도 |
| `getMarketPrice(종목코드, 시간)` | 현재가 조회 |

### 2.2 Trading System 기본 기능

| 함수 | 설명 |
| --- | --- |
| `selectStockBrocker()` | 키워 또는 네모 증권사 선택 |
| `login(id, pass)` | 로그인 |
| `buy(종목코드, 가격, 수량)` | 매수 |
| `sell(종목코드, 가격, 수량)` | 매도 |
| `getPrice(종목코드)` | 현재가 조회 |

### 2.3 Trading System 핵심 기능

#### 기능 1: `buyNiceTiming(종목, 총 금액)`

- 200ms 주기로 3회 가격을 읽고, 가격이 올라가는 추세인지 파악한다.
- 가격이 올라가는 추세라면, 총 금액을 최대한 사용하여 최대 수량만큼 매수한다.
- 마지막에 읽은 가격으로 매수한다.

#### 기능 2: `sellNiceTiming(종목, 수량)`

- 200ms 주기로 3회 가격을 읽고, 가격이 내려가는 추세인지 파악한다.
- 가격이 내려가는 추세라면, 사용자가 설정한 수량만큼 주식을 모두 매도한다.
- 마지막에 읽은 가격으로 매도한다.

### 2.4 설계 원칙

- 각 증권사 API는 OCP(개방-폐쇄 원칙)를 위해 `StockBrocker` 인터페이스로 감싸서 Trading System이 사용한다.
