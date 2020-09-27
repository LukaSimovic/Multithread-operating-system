/*
 * pcb.h
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include"thread.h"
#include <iostream.h>

class ListPCB;
class ListHand;

class PCB{
public:
	PCB(Thread* t, StackSize stackSize , Time timeSlice);
	~PCB();

	unsigned* stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;
	unsigned quantum;

	enum State{ READY, BLOCKED, FINISHED, CREATED};
	State state;

	static int gr; //za ID
	ID id;

	static volatile PCB* running;


	void waitToComplete();
	static void wrapper();


	static ListPCB* allPCBlist;
	ListPCB* waitingList;

	int timeBlockingFlag; //da li se nit zablokirala na neko vreme
	unsigned semBlockingTime; //na koje vreme


	//ispis PCB-a (pomaze pri ispisu liste PCB-ova)
	friend ostream& operator<<(ostream& os, PCB* p){
			os<<"Thread: "<<p->id<<endl;
			os<<"Status: "<<p->state<<endl;
			os<<"TimeSlice: "<<p->quantum<<endl;
			return os;
		}


	// ****SIGNALI*****
	void signal(SignalId signal);
	void registerHandler(SignalId signal, SignalHandler handler);
	void unregisterAllHandlers(SignalId id);
	void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
	void blockSignal(SignalId signal);
	static void blockSignalGlobally(SignalId signal);
	void unblockSignal(SignalId signal);
	static void unblockSignalGlobally(SignalId signal);


	ListHand* signalInfo[16];

	int orderOfSignals[16]; //redosled kojim dolaze
	int cntSignal; //broj signala za obradu

	ID myParentId;
	int wrapperFlag; //za signale 1 i 2

	void handleSignals();

	void writeSignals();

protected:
	friend class Thread;
	friend class Settings;
	friend class ListPCB;

private:
	Thread* myThread;
	StackSize stSize;

	static void sendSystemSignals();
};


#endif /* PCB_H_ */
