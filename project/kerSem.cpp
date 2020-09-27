/*
 * kerSem.cpp
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#include"kerSem.h"
#include"settings.h"
#include"listSem.h"
#include"listPCB.h"
#include"SCHEDULE.H"

ListSem* KernelSem::allSemList= new ListSem();

KernelSem::KernelSem(int init){
	lock
	val=init;

	blockedList = new ListPCB();

	unlock
}

int KernelSem::wait(Time mTime){
	lock
	int ret=1;
	val--;
	if(val<0){
		PCB::running->state=PCB::BLOCKED;
		if(mTime>0){
			PCB::running->timeBlockingFlag=1;
			PCB::running->semBlockingTime=mTime;
			ret=0;
		}
		else{
			ret=1;
		}
		blockedList->add((PCB*)PCB::running);
		unlock
		dispatch();
		if(PCB::running->timeBlockingFlag==0){
			ret=1;
		}
		return ret;
	}
	unlock
	return ret;
}

int KernelSem::signal(int n){
	lock
	int ret=0;
	int toUnblock;
	if(n>0){
		toUnblock=n;
		val+=n;
	}
	else if(n==0){
		toUnblock=1;
		val+=1;
		ret=0;
	}
	else{
		unlock
		return n;
	}

	if(toUnblock>blockedList->size())
		toUnblock= blockedList->size();

	int i;
	PCB* p;
	for(i=0; i<toUnblock; i++){
		p = blockedList->removeFirst();
		p->state=PCB::READY;
		Scheduler::put(p);
		p->timeBlockingFlag=0;
		if(n!=0)
			ret++;
	}
	unlock;
	return ret;
}

KernelSem::~KernelSem(){
	lock
		allSemList->removeElem(this);
		delete blockedList;
	unlock
}

int KernelSem::getVal(){
	return val;
}

void KernelSem::updateVal(int t){
	val+=t;
}
