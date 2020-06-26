#ifndef NATIVE
#include <Arduino.h>
#endif

#define BURP_UNITY_MAX_MODULE 1
#define BURP_UNITY_MAX_DESCRIBE 3
#define BURP_UNITY_MAX_POP 5
#define BURP_UNITY_MAX_TEST 1
#define BURP_UNITY_MAX_IT 1
#define BURP_UNITY_MAX_ASYNC_IT 1
#define BURP_UNITY_MAX_CALLBACK 1
#define BURP_UNITY_MAX_ASYNC_CALLBACK 1
#define BURP_UNITY_MAX_STACKED_CALLBACK 1
#define BURP_UNITY_MAX_STACKED_ASYNC_CALLBACK 1
#define BURP_UNITY_MAX_SETUP 1
#define BURP_UNITY_MAX_DEPTH 5
#define BURP_UNITY_MAX_BEFORE_EACH 1
#define BURP_UNITY_MAX_ASYNC_BEFORE_EACH 1
#define BURP_UNITY_MAX_AFTER_EACH 1
#define BURP_UNITY_MAX_ASYNC_AFTER_EACH 1
#define BURP_UNITY_MAX_LOOP 1

#include <unity.h>
#include <BurpUnity.hpp>

#include "Action.hpp"
#include "Store.hpp"
#include "Combined.hpp"

Runner<3> runner({
  &BurpReduxTest::actionTests,
  &BurpReduxTest::storeTests,
  &BurpReduxTest::combinedTests
});
Memory memory;
bool running = true;
int status;

void setup() {
  UNITY_BEGIN();
  // BurpUnity::Usage::printParams(memory);
  runner.setup(memory);
}

void loop() {
  if (running) {
    runner.loop();
    if (runner.isFinished()) {
      // BurpUnity::Usage::printUsage(memory, runner);
      // BurpUnity::Usage::printUsageDefines(memory, runner);
      status = UNITY_END();
      running = false;
    }
  }
}

int main() {
  setup();
  while (running) {
    loop();
  }
  return status;
}
