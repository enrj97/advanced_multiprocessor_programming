#include "petersonLock.hpp"

PetersonLock::PetersonLock(int n){
	numOfThreads = n;
	root = new PetersonNode(NULL, numOfThreads);

	std::vector<PetersonNode*> initList;
	initList.push_back(root);

	leaves = createTree(initList);
}

PetersonLock::~PetersonLock(){
}

void PetersonLock::lock() {
	int i = omp_get_thread_num();
	PetersonNode* currentNode = getLeaf(i);

	while (currentNode != NULL) {
		currentNode->lock();
		currentNode = currentNode->parent;
	}
}

void PetersonLock::unlock() {
	int i = omp_get_thread_num();
	PetersonNode* currentNode = getLeaf(i);

	while (currentNode != NULL) {
		currentNode->unlock();
		currentNode = currentNode->parent;
	}
}

// get leaf for a specific thread id
PetersonNode* PetersonLock::getLeaf(int num) {
	//each lock shares two threads.
	return leaves.at(num / 2);
}

//create leafs untile we have currentLeaves.size() = numOfThreads/2 
std::vector<PetersonNode*> PetersonLock::createTree(std::vector<PetersonNode*> nodes) {
	// 	When the number of leaves we have is == 1/2 * number_of_threads
	if ((int)nodes.size() == numOfThreads / 2)
		return nodes;

	// This array will be the current leaves in the iteration for creating the PetersonTree
	std::vector<PetersonNode*> currentLeaves;

	// Loop through each node, add two childent and store them as the current leaves
	for (PetersonNode* node : nodes) {
		node->leftChild = new PetersonNode(node, numOfThreads);
		node->rightChild = new PetersonNode(node, numOfThreads);

		currentLeaves.push_back(node->leftChild);
		currentLeaves.push_back(node->rightChild);
	}

	// Recurse, passing our current leaves back to the function
	return createTree(currentLeaves);
}
