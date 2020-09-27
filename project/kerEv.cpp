/*
 * kerEv.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */

#include"kerEv.h"
#include"settings.h"
#include"SCHEDULE.H"
#include"pcb.h"
#include"IVTEntry.h"

KernelEv::KernelEv(IVTNo ivtNo){

	val=0;
	numEntry=ivtNo;
	owner = (PCB*)PCB::running;
	IVTEntry::IVTable[ivtNo]->myKernelEv = this;

}

void KernelEv::wait(){

	if(owner == PCB::running){
		if(val==0){ //block
			owner->state=PCB::BLOCKED;
			dispatch();
			return;
		}else if(val==1){
			val=0;
		}
	}

}

void KernelEv::signal(){

	if(val==0){
		if(owner->state==PCB::BLOCKED){
			owner->state=PCB::READY;
			Scheduler::put(owner);
			dispatch();
		}else{
			val=1;
		}

	}


}


KernelEv::~KernelEv(){
	owner=0;
	IVTEntry::IVTable[numEntry]->myKernelEv = 0;


}
