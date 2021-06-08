#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <climits>
#include <vector>
#include "baseLock.hpp"
#include "petersonNode.hpp"

class PetersonLock: public BaseLock {
  private:
      int numOfThreads;
      PetersonNode* getLeaf(int num);
      std::vector<PetersonNode*> createTree(std::vector<PetersonNode*> nodes);

  public:
    PetersonLock(int n);
    ~PetersonLock();
    void lock();
    void unlock();

    PetersonNode *root;
    std::vector<PetersonNode*> leaves;

    const char *get_name() {
      return "PetersonLock";
    };

};
