/*
 * listSem.h
 *
 *  Created on: Aug 14, 2020
 *      Author: OS1
 */

#ifndef LISTSEM_H_
#define LISTSEM_H_

#include"kerSem.h"
#include<iostream.h>

class ListSem{

public:
	ListSem() : first(0), last(0), cntSem(0) {}
	~ListSem();

	void add(KernelSem* k);

	void updateTimeList();
	void removeElem(KernelSem* k);


	friend ostream& operator<<(ostream& os, ListSem* p){

		if(p->cntSem==0)
			os<<"List is empty!"<<endl;
		else{
			Elem* cur = p->first;
			os<<"Size: "<<p->cntSem<<endl;
			while(cur){
				os<<cur->kSem<<endl;
				cur=cur->next;
			}
		}
		return os;
	}


private:
	struct Elem{
		KernelSem* kSem;
		Elem* next;
		Elem(KernelSem* k){
			kSem=k;
			next=0;
		}
	};

	Elem *first, *last;
	int cntSem;
};


#endif /* LISTSEM_H_ */
