#ifndef NATIVE
#include <Arduino.h>
#endif

#define BDD_UNITY_MAX_MODULE 1
#define BDD_UNITY_MAX_DESCRIBE 3
#define BDD_UNITY_MAX_POP 2
#define BDD_UNITY_MAX_TEST 2
#define BDD_UNITY_MAX_IT 1
#define BDD_UNITY_MAX_ASYNC_IT 1
#define BDD_UNITY_MAX_CALLBACK 2
#define BDD_UNITY_MAX_ASYNC_CALLBACK 1
#define BDD_UNITY_MAX_STACKED_CALLBACK 1
#define BDD_UNITY_MAX_STACKED_ASYNC_CALLBACK 1
#define BDD_UNITY_MAX_SETUP 1
#define BDD_UNITY_MAX_DEPTH 2
#define BDD_UNITY_MAX_BEFORE_EACH 1
#define BDD_UNITY_MAX_ASYNC_BEFORE_EACH 1
#define BDD_UNITY_MAX_AFTER_EACH 1
#define BDD_UNITY_MAX_ASYNC_AFTER_EACH 1
#define BDD_UNITY_MAX_LOOP 1

#include <unity.h>
#include <BddUnity.hpp>

#include "Action.hpp"
#include "SubscriberList.hpp"
#include "Store.hpp"
#include "Selector.hpp"

Runner<4> runner({
  &CppReduxTest::Action::tests,
  &CppReduxTest::SubscriberList::tests,
  &CppReduxTest::Store::tests,
  &CppReduxTest::Selector::tests
});
Memory memory;
bool running = true;
int status;

void setup() {
  UNITY_BEGIN();
  // BddUnity::Usage::printParams(memory);
  runner.setup(memory);
}

void loop() {
  if (running) {
    runner.loop();
    if (runner.isFinished()) {
      // BddUnity::Usage::printUsage(memory, runner);
      // BddUnity::Usage::printUsageDefines(memory, runner);
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
