#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <omp.h>
#include <time.h>
#include <unistd.h>
#include <climits>

class PetersonNode{
  private:
    bool* flags;
    int victim;
    bool flagCheck();
    int nThreads;

  public:
    PetersonNode(PetersonNode* node, int n);
    ~PetersonNode ();
    void lock();
    void unlock();
    PetersonNode* leftChild;
    PetersonNode* rightChild;
    PetersonNode* parent;

};
