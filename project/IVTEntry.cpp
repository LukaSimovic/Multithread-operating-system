/*
 * IVTEntry.cpp
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */


#include"IVTEntry.h"
#include"settings.h"
#include<dos.h>

IVTEntry* IVTEntry::IVTable[256];

IVTEntry::IVTEntry(IVTNo num, pInterrupt newRout){
	lock
	numEntry=num;
	IVTable[num] = this;
#ifndef BCC_BLOCK_IGNORE
	oldRout = getvect(num);
	setvect(num, newRout);
#endif
	myKernelEv=0;
	unlock
}

void IVTEntry::callOldRout(){
	lock
		(*oldRout)();
	unlock
}

void IVTEntry::signal(){

	myKernelEv->signal();

}

void IVTEntry::restoreIVT(){
	lock
		#ifndef BCC_BLOCK_IGNORE
			setvect(numEntry, oldRout);
		#endif
			IVTable[numEntry]=0;
	unlock
}

IVTEntry::~IVTEntry(){
	(*oldRout)();
	restoreIVT();

}
