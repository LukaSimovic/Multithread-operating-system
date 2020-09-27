/*
 * semaphor.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#include"semaphor.h"
#include"kerSem.h"
#include"listSem.h"
#include"settings.h"

Semaphore::Semaphore(int init){
	myImpl = new KernelSem(init);

	KernelSem::allSemList->add(myImpl);
}

int Semaphore::wait(Time maxTimeToWait){
	lock
		int ret=0;
		ret = myImpl->wait(maxTimeToWait);
	unlock
	return ret;

}

int Semaphore::signal(int n){
	lock
		int ret=0;
		ret = myImpl->signal(n);
	unlock
	return ret;
}

Semaphore::~Semaphore(){
	lock
		delete myImpl;
	unlock
}

int Semaphore::val () const{

	lock
		int ret=0;
		ret = myImpl->getVal();
	unlock
	return ret;
}
