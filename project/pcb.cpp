/*
 * pcb.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */


#include"pcb.h"
#include<dos.h>
#include"listPCB.h"
#include"settings.h"
#include<iostream.h>
#include<stdio.h>
#include"idle.h"
#include"listHand.h"

int PCB::gr=0;

volatile PCB* PCB::running=0;
ListPCB* PCB::allPCBlist=new ListPCB();

PCB::PCB(Thread* t, StackSize stackSize , Time timeSlice){
	id=++gr;
	myThread=t;
	if(stackSize>65535){
		stSize=65535/sizeof(unsigned);
	}else{
		stSize=stackSize/sizeof(unsigned);
	}
	stack = new unsigned[stSize];

	stack[stSize-1]=0x200;
#ifndef BCC_BLOCK_IGNORE
	stack[stSize-2]=FP_SEG(&(wrapper)); //wrapper seg
	stack[stSize-3]=FP_OFF(&(wrapper)); //wrapper off
	ss=FP_SEG(stack+stSize-12);
	sp=FP_OFF(stack+stSize-12);
#endif
	bp=sp;


	quantum=timeSlice;
	state = PCB::CREATED;

	waitingList = new ListPCB();

	semBlockingTime=0;
	timeBlockingFlag=0;

	cntSignal=0;

	int l;
	if(id ==1){
		myParentId=0;

		for(l=0; l<16; l++){
			signalInfo[l] = new ListHand();
			orderOfSignals[l]=-1;
		}

	}else{ //i have parent (not global object)
		myParentId =PCB::running->id;

		for(l=0; l<16; l++){
			signalInfo[l] = new ListHand();
			signalInfo[l]->copyParentInfo(Thread::getThreadById(myParentId)->myPCB->signalInfo[l]);
			orderOfSignals[l]=Thread::getThreadById(myParentId)->myPCB->orderOfSignals[l];
		}
	}


	wrapperFlag=0;

}

PCB::~PCB(){

	lock

	//allPCBlist->removeElem(this);

	delete waitingList;
	delete [] stack;

	for(int i=0; i<16; i++)
		delete signalInfo[i];

	unlock
}

void PCB::waitToComplete(){
		lock

		if(this == Settings::idleThread->getIdlePCB()){
			unlock
			return;
		}
		if(PCB::running == this){
			unlock
			return;
		}
		if(this->state == PCB::FINISHED){
			unlock
			return;
		}
		if(this->state == PCB::CREATED){
			unlock
			return;
		}

		PCB::running->state=PCB::BLOCKED;
		this->waitingList->add((PCB*)PCB::running);

		unlock
		dispatch();
}

void PCB::wrapper(){
	PCB::running->myThread->run();
	lock
	PCB::running->wrapperFlag=1;
	PCB::sendSystemSignals();
	PCB::running->wrapperFlag=0;
	unlock
	exitThread();
}

void PCB::sendSystemSignals(){

		if(PCB::running->myParentId !=0){
			Thread::getThreadById(PCB::running->myParentId)->signal(1);
		}

		((PCB*)PCB::running)->signal(2);


}

void PCB::signal(SignalId sig){

	if(sig>=0 && sig<16){
		if(((sig==1 || sig==2) && wrapperFlag==1) || (sig!=1 && sig!=2)){
			if(PCB::running != this || signalInfo[sig]->blocked==1){
				orderOfSignals[cntSignal]=sig;
				cntSignal++;
			}else{
				if(sig!=0)
					signalInfo[sig]->callAllHandlers();
				else
					Settings::killThread();
			}
		}
	}
}

void PCB::registerHandler(SignalId sig, SignalHandler handler){

	if(sig>=0 && sig<16){
			signalInfo[sig]->add(handler);
	}
}

void PCB::unregisterAllHandlers(SignalId id){

	if(id>=0 && id<16){
			signalInfo[id]->unregisterHandlers();
	}
}

void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){

	if(id>=0 && id<16){
			signalInfo[id]->swap(hand1, hand2);
	}
}

void PCB::blockSignal(SignalId sig){

	if(sig>=0 && sig<16){
			signalInfo[sig]->blocked=1;

	}
}

void PCB::blockSignalGlobally(SignalId sig){

	if(sig>=0 && sig<16){
			PCB::allPCBlist->blockSignal(sig);
	}
}

void PCB::unblockSignal(SignalId sig){

	if(sig>=0 && sig<16){
		signalInfo[sig]->blocked=0;
	}
}

void PCB::unblockSignalGlobally(SignalId sig){

	if(sig>=0 && sig<16){
		PCB::allPCBlist->unblockSignal(sig);
	}
}

void PCB::handleSignals(){

	int i;
	int o;
	int blocked[16];
	int cntBlocked=0;

	for(i=0; i<cntSignal; i++){
		o = orderOfSignals[i];
		if(signalInfo[o]->blocked==0){

			if(o!=0){
				Settings::lockFlag=1;
				signalInfo[o]->callAllHandlers();
				Settings::lockFlag=0;
			} else{
				Settings::lockFlag=1;
				Settings::killThread();
				Settings::lockFlag=0;
			}

		}else{
			blocked[cntBlocked]=o;
			cntBlocked++;
		}
	}

	cntSignal=cntBlocked;

	for(i=0; i<16; i++){
		if(i<cntSignal)
			orderOfSignals[i]=blocked[i];
		else
			orderOfSignals[i]=-1;
	}


	if(PCB::running->state == PCB::FINISHED){
			dispatch();
	}



}




void PCB::writeSignals(){
	for(int i=0; i<16; i++){
		lock
			cout<<"SIGNAL: "<<i<<endl;
			cout<<signalInfo[i]<<endl;
			cout<<endl;
		unlock
	}
}
