#include <atomic>
#include "baseLock.hpp"

class TestAndTestAndSetLock: public BaseLock {

  private:
    std::atomic<bool> state;

  public:
    TestAndTestAndSetLock(int n);
    ~TestAndTestAndSetLock();
    void lock();
    void unlock();

    const char *get_name() {
      return "TTAS Lock";
    };

};
