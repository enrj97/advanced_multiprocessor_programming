
/*
 *	 Simple Test-And-Set Lock
 */

#include "tas.hpp"

TestAndSetLock::TestAndSetLock(int n){
	state=false;
};

TestAndSetLock::~TestAndSetLock(){}

void TestAndSetLock::lock(){
	while(state.exchange(true)){}
};

void TestAndSetLock::unlock(){
	state.exchange(false);
};