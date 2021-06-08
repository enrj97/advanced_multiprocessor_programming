#include "petersonNode.hpp"

PetersonNode::PetersonNode(PetersonNode* node, int n) {
	flags = new bool[n];
	for (int i = 0; i < n; i ++) {
	  flags[i] = false;
	}
	parent = node;
	nThreads = n;
}

PetersonNode::~PetersonNode() {
	delete[] flags;
}

void PetersonNode::lock () {
	int i = omp_get_thread_num();
	flags[i] = true;
	victim = i;
	while(flagCheck() && victim == i) { /* wait */ }
}
	

void PetersonNode::unlock () {
	flags[omp_get_thread_num()] = false;
}

bool PetersonNode::flagCheck() {
	int me = omp_get_thread_num();
	for (int i = 0; i < nThreads; i++) {
		if (flags[i] && (i != me))
			return true;
	}
	return false;
}