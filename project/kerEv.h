/*
 * kerEv.h
 *
 *  Created on: Aug 15, 2020
 *      Author: OS1
 */

#ifndef KEREV_H_
#define KEREV_H_

#include"event.h"

class PCB;
class IVTEntry;

class KernelEv{

public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();

	void wait();
	void signal();

	int val;
	PCB* owner;

private:
	IVTNo numEntry;
};


#endif /* KEREV_H_ */
