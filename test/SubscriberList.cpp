#include <unity.h>
#include "Subscriber.hpp"
#include "SubscriberList.hpp"

namespace CppReduxTest {
  namespace SubscriberList {

    using Subscriber = Subscriber::Subscriber;

    Subscriber subscriber1;
    Subscriber subscriber2;
    Subscriber subscriber3;
    CppRedux::Subscribers<3> subscribers = {
      &subscriber1,
      &subscriber2,
      &subscriber3
    };
    CppRedux::SubscriberList<3> subscriberList(subscribers);
    
    Module tests("SubscriberList", [](Describe & describe) {
        describe.loop([]() {
            subscriber1.loop();
            subscriber2.loop();
            subscriber3.loop();
        });

        describe.async("notify", [](Async & async, f_done & done) {
            Subscriber::f_cb cb = [&]() {
              static int count = 0;
              count++;
              if (count == 3) {
                async.it("should notify all the subscribers", []() {
                    TEST_ASSERT_TRUE(true);
                });
                done();
              }
            };
            subscriber1.callbackOnce(cb);
            subscriber2.callbackOnce(cb);
            subscriber3.callbackOnce(cb);
            subscriberList.notify();
        });
    });

  }
}
