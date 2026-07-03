#pragma once

#include "gmock/gmock.h"
#include "Logger.h"

// ILogger interface를 검증하기 위한 Mock 구현체
class MockLogger : public ILogger {
public:
	MOCK_METHOD(void, log, (const std::string& message), (override));
};
