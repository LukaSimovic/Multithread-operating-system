/*
 * listPCB.h
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#ifndef LISTPCB_H_
#define LISTPCB_H_

#include"pcb.h"
#include <iostream.h>
#include<stdio.h>
#include"listHand.h"


class ListPCB{
public:

	ListPCB() :  first(0),last(0), cntPCB(0) {}
	~ListPCB();

	int size(){ return cntPCB;}

	void add(PCB* p);
	Thread* findById(ID pid);
	void unblockWaitingList();
	void removeById(ID pid);
	PCB* removeFirst();
	void removeElem(PCB* p);


	int updateTime(); //svim semaforima koji su zablokirani na neko vreme smanjuje timeSlice za 1


	//ispis liste PCB-ova
	friend ostream& operator<<(ostream& os, ListPCB* p){
		if(p==0)
			os<<"List doesn't exist!";
		else{
			os<<"Size: "<<p->cntPCB<<endl;
			Elem* cur = p->first;
			while(cur){
					os<<cur->pcb<<endl;
					cur=cur->next;
			}

		}
		return os;
	}


	//globalno blokiranje signala
	void blockSignal(SignalId sid);
	void unblockSignal(SignalId sid);

private:

	struct Elem{
		PCB* pcb;
		Elem* next;
		Elem(PCB* p) {
			pcb=p;
			next=0;
		}
	};

	Elem *first, *last;
	int cntPCB;
};



#endif /* LISTPCB_H_ */
