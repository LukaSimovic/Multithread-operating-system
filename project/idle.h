/*
 * idle.h
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#ifndef IDLE_H_
#define IDLE_H_

#include"thread.h"

class PCB;

class Idle : public Thread {
public:
	Idle() : Thread(defaultStackSize,1) {}
	PCB* getIdlePCB();
protected:
	virtual void run();
};

#endif /* IDLE_H_ */
