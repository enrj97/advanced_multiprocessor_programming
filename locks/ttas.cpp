/*
 *	 Simple Test-And-Test-And-Set Lock
 */

#include "ttas.hpp"

TestAndTestAndSetLock::TestAndTestAndSetLock(int n){
	state=false;
};

TestAndTestAndSetLock::~TestAndTestAndSetLock(){}

void TestAndTestAndSetLock::lock(){
	while(true){
		
		while(state){}
		
		if(!state.exchange(true))
			return;
	}
};

void TestAndTestAndSetLock::unlock(){
	state.exchange(false);
};