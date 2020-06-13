#ifndef NATIVE
#include <Arduino.h>
#endif

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
