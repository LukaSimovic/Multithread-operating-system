/*
 * event.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */


#include"event.h"
#include"settings.h"
#include"kerEv.h"

Event::Event(IVTNo ivtNo){
	lock
		myImpl = new KernelEv(ivtNo);
	unlock
}

void Event::wait(){
	lock
		myImpl->wait();
	unlock
}

void Event::signal(){
	lock
		myImpl->signal();
	unlock
}

Event::~Event(){
	lock
		delete myImpl;
	unlock
}

