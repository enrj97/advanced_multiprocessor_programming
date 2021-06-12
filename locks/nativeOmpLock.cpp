#include "nativeOmpLock.hpp"

NativeOmpLock::NativeOmpLock () {
	omp_init_lock(&lck);
}
NativeOmpLock::~NativeOmpLock () {
	omp_destroy_lock(&lck);
}

void NativeOmpLock::lock () {
	omp_set_lock(&lck);
}

void NativeOmpLock::unlock () {
	omp_unset_lock(&lck);
}

