#ifndef NATIVE
#include <Arduino.h>
#endif

// #define BDD_UNITY_MAX_MODULE 5
// #define BDD_UNITY_MAX_DESCRIBE 7
// #define BDD_UNITY_MAX_POP 4
// #define BDD_UNITY_MAX_TEST 2
// #define BDD_UNITY_MAX_IT 1
// #define BDD_UNITY_MAX_ASYNC_IT 1
// #define BDD_UNITY_MAX_CALLBACK 1
// #define BDD_UNITY_MAX_ASYNC_CALLBACK 0
// #define BDD_UNITY_MAX_STACKED_CALLBACK 0
// #define BDD_UNITY_MAX_STACKED_ASYNC_CALLBACK 0
// #define BDD_UNITY_MAX_SETUP 1
// #define BDD_UNITY_MAX_DEPTH 4
// #define BDD_UNITY_MAX_BEFORE_EACH 0
// #define BDD_UNITY_MAX_ASYNC_BEFORE_EACH 0
// #define BDD_UNITY_MAX_AFTER_EACH 0
// #define BDD_UNITY_MAX_ASYNC_AFTER_EACH 0
// #define BDD_UNITY_MAX_LOOP 1

#include <unity.h>
#include <BddUnity.hpp>

#include "Action.hpp"
#include "State.hpp"
#include "CombinedReducer.hpp"
#include "SubscriberList.hpp"
#include "Store.hpp"

Runner<5> runner({
  &CppReduxTest::Action::tests,
  &CppReduxTest::State::tests,
  &CppReduxTest::CombinedReducer::tests,
  &CppReduxTest::SubscriberList::tests,
  &CppReduxTest::Store::tests
});
Memory memory;
bool running = true;
int status;

void setup() {
  UNITY_BEGIN();
  BddUnity::Usage::printParams(memory);
  runner.setup(memory);
}

void loop() {
  if (running) {
    runner.loop();
    if (runner.isFinished()) {
      BddUnity::Usage::printUsage(memory, runner);
      BddUnity::Usage::printUsageDefines(memory, runner);
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
