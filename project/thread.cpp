/*
 * thread.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#include"thread.h"
#include"pcb.h"
#include"SCHEDULE.H"
#include"listPCB.h"
#include"settings.h"
#include<stdio.h>
#include"idle.h"
#include<iostream.h>

Thread::Thread(StackSize stackSize, Time timeSlice){
	myPCB = new PCB(this, stackSize, timeSlice);

	PCB::allPCBlist->add(myPCB);

}


void Thread::start(){
	if(myPCB->state == PCB::CREATED){
		myPCB->state=PCB::READY;
		if(myPCB != Settings::idleThread->getIdlePCB())
			Scheduler::put(myPCB);
	}

}

Thread::~Thread(){
	lock

		waitToComplete();
		PCB::allPCBlist->removeById(myPCB->id);
		myPCB->waitingList->unblockWaitingList();
		delete myPCB;

	unlock
}


Thread* Thread::getThreadById(ID id){
	Thread* ret=0;
	ret = PCB::allPCBlist->findById(id);
	return ret;
}

ID Thread::getRunningId(){
	return PCB::running->id;
}

ID Thread::getId(){
	return myPCB->id;
}

void Thread::waitToComplete(){
		myPCB->waitToComplete();
}

// ****************************SIGNALI****************************

void Thread::signal(SignalId sig){
	Settings::lockFlag=1;
		myPCB->signal(sig);
	Settings::lockFlag=0;
}

void Thread::registerHandler(SignalId sig, SignalHandler h){
	Settings::lockFlag=1;
		myPCB->registerHandler(sig,h);
	Settings::lockFlag=0;

}

void Thread::unregisterAllHandlers(SignalId id){
	Settings::lockFlag=1;
		myPCB->unregisterAllHandlers(id);
	Settings::lockFlag=0;
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	Settings::lockFlag=1;
		myPCB->swap(id,hand1,hand2);
	Settings::lockFlag=0;
}

void Thread::blockSignal(SignalId sig){
		myPCB->blockSignal(sig);
}

void Thread::blockSignalGlobally(SignalId sig){
	Settings::lockFlag=1;
		PCB::blockSignalGlobally(sig);
	Settings::lockFlag=0;
}

void Thread::unblockSignal(SignalId sig){
		myPCB->unblockSignal(sig);
}

void Thread::unblockSignalGlobally(SignalId sig){
	Settings::lockFlag=1;
		PCB::unblockSignalGlobally(sig);
	Settings::lockFlag=0;
}

