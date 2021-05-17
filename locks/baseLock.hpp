#ifndef BASELOCK_H
#define BASELOCK_H

class BaseLock {
  public:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual const char *get_name() = 0;
};


#endif // BASELOCK_H
