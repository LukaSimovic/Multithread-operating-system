/*
 * listPCB.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#include"listPCB.h"
#include"settings.h"
#include"SCHEDULE.H"



void ListPCB::add(PCB* p){
	lock
	Elem* n = new Elem(p);

	if(first==0)
		first=n;
	else
		last->next=n;
	last=n;

	cntPCB++;

	unlock

}

ListPCB::~ListPCB(){

	Elem* cur=first;
	lock
		while(cur!=0){
			Elem* old=cur;
			cur=cur->next;
			old->next=0;
			delete old;
		}
		first=last=0;
		cntPCB=0;
		unlock

}



Thread* ListPCB::findById(ID pid){

	Thread* ret=0;
	Elem* cur=first;
	lock
	while(cur!=0){
		if(cur->pcb->id == pid){
			unlock
			return cur->pcb->myThread;
		}else{
			cur=cur->next;
		}
	}
	unlock
	return ret;

}



void ListPCB::unblockWaitingList(){

		lock
		if(cntPCB!=0){

			Elem* cur=first;
			while(cur!=0){
				Elem* old = cur;
				PCB* oldPCB=old->pcb;
				cur=cur->next;

				oldPCB->state=PCB::READY;
				Scheduler::put(oldPCB);

				delete old;
			}
			first=last=0;
			cntPCB=0;

		}
		unlock

}

void ListPCB::removeById(ID pid){


	Elem* rem = 0;
	Elem* pret=0;
	Elem* cur = first;
	lock
	while(cur!=0){
		if(cur->pcb->id==pid){
			rem=cur;
			break;
		}else{
			pret=cur;
			cur=cur->next;
		}
	}
	if(rem!=0){
		if(rem==first){
			first=rem->next;
			if(first==0)
				last=0;

		}else if(rem==last){
			pret->next=0;
			last=pret;

		}else{
			pret->next=rem->next;
		}
		//rem->next=0;
		delete rem;
		cntPCB--;
	}
	unlock

}



PCB* ListPCB::removeFirst(){

	lock
	PCB* p = 0;
	Elem* cur=first;
	if(cur!=0){
		p= cur->pcb;
		first=first->next;
		cur->next=0;
		delete cur;
		if(first==0)
			last=0;
		cntPCB--;
	}
	unlock
	return p;


}

void ListPCB::removeElem(PCB *p){
	Elem* rem = 0;
	Elem* pret=0;
	Elem* cur = first;
	lock
	while(cur!=0){
		if(cur->pcb == p){
			rem=cur;
			break;
		}else{
			pret=cur;
			cur=cur->next;
		}
	}
	if(rem!=0){
		if(rem==first){
			first=rem->next;
			if(first==0)
				last=0;
		}else if(rem==last){
			pret->next=0;
			last=pret;
		}else{
			pret->next=rem->next;
		}
		delete rem;
		cntPCB--;
	}
	unlock
}



int ListPCB::updateTime(){
	int ret=0;
	int change=0;
	Elem* cur=first;
	lock
	while(cur!=0){
		change=0;
		if(cur->pcb->timeBlockingFlag==1){
			PCB* p =cur->pcb;
			p->semBlockingTime--;

			if(p->semBlockingTime==0){
				p->state=PCB::READY;
				Scheduler::put(p);
				ID pid = p->id;
				cur=cur->next;
				change=1;
				this->removeById(pid);
				ret++;
			}
		}
		if(change==0)
			cur=cur->next;
	}
	unlock
	return ret;
}

// **************************SIGNALI*********************

void ListPCB::blockSignal(SignalId sid){
	Elem* cur=first;
	lock
	while(cur!=0){
		cur->pcb->signalInfo[sid]->blocked=1;
		cur=cur->next;
	}
	unlock
}

void ListPCB::unblockSignal(SignalId sid){
	Elem* cur=first;
	lock
	while(cur!=0){
		cur->pcb->signalInfo[sid]->blocked=0;
		cur=cur->next;
	}
	unlock
}

