/*
 * listSem.cpp
 *
 *  Created on: Aug 14, 2020
 *      Author: OS1
 */

#include"listSem.h"
#include"kerSem.h"
#include"listPCB.h"
#include"settings.h"

void ListSem::add(KernelSem* k){
	lock
		Elem* n = new Elem(k);

		if(first==0)
			first=n;
		else
			last->next=n;
		last=n;

		cntSem++;
	unlock
}

ListSem::~ListSem(){
	Elem* cur=first;
	lock
			while(cur!=0){
				Elem* old=cur;
				cur=cur->next;
				old->next=0;
				delete old;
			}
			first=last=0;
			cntSem=0;
	unlock
}

void ListSem::updateTimeList(){
	int temp=0;
	Elem* cur=first;
	lock
	while(cur!=0){
		temp = cur->kSem->blockedList->updateTime();
		cur->kSem->updateVal(temp);
		cur=cur->next;
	}
	unlock
}

void ListSem::removeElem(KernelSem *k){
	Elem* rem = 0;
	Elem* pret=0;
	Elem* cur = first;
	lock
	while(cur!=0){
		if(cur->kSem == k){
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
		cntSem--;
	}
	unlock
}
