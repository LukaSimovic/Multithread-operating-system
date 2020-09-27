/*
 * IVTEntry.h
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include"kerEv.h"

typedef void interrupt (*pInterrupt)(...);

#define PREPAREENTRY(number, oldRoutineFlag)\
	void interrupt intr##number(...);\
	IVTEntry ivtEntry##number(number,intr##number);\
	void interrupt intr##number(...){\
		IVTEntry::IVTable[number]->signal();\
		if (oldRoutineFlag == 1){\
			IVTEntry::IVTable[number]->oldRout();\
			}\
	}\

class IVTEntry{
public:

	IVTEntry(IVTNo num, pInterrupt newRout);
	~IVTEntry();

	pInterrupt oldRout;

	IVTNo numEntry;

	static IVTEntry* IVTable[256];

	void callOldRout();
	void signal();

protected:
	friend class KernelEv;


private:
	void restoreIVT(); //zovem u destruktoru
	KernelEv* myKernelEv;

};



#endif /* IVTENTRY_H_ */
