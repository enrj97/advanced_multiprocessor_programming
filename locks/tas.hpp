#include <atomic>
#include "baseLock.hpp"

class TestAndSetLock: public BaseLock {

  private:
    std::atomic<bool> state;

  public:
    TestAndSetLock(int n);
    ~TestAndSetLock();
    void lock();
    void unlock();

    const char *get_name() {
      return "TAS Lock";
    };

};
