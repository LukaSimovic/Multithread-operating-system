/*
 * ListHand.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */


/*
 * ListHand.cpp
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */


#include"settings.h"
#include<iostream.h>
#include "listHand.h"



ListHand::ListHand(){
	first=0;
	last=0;
	cntHand=0;
	//arrived=0;
	//order=-1;
	blocked=0;
}

ListHand::~ListHand(){
	Elem* cur=first;

	while(cur!=0){
		Elem* old=cur;
		cur=cur->next;
		old->next=0;
		delete old;
	}
	first=last=0;
	cntHand=0;

}

void ListHand::add(SignalHandler sh){

	Elem* n = new Elem(sh);

	if(first==0)
		first=n;
	else
		last->next=n;
	last=n;

	cntHand++;
}

void ListHand::callAllHandlers(){

	Elem* cur=first;

	while(cur!=0){
		SignalHandler hand = cur->sh;
		(*hand)();
		cur=cur->next;
	}

}

void ListHand::unregisterHandlers(){
	Elem* cur=first;

				while(cur!=0){
					Elem* old=cur;
					cur=cur->next;
					old->next=0;
					delete old;
				}
				first=last=0;
				cntHand=0;

}

void ListHand::swap(SignalHandler h1, SignalHandler h2){


	Elem* cur=first;
	Elem* firstFound=0;
	SignalHandler temp;
	while(cur!=0){

		if(cur->sh==h1 || cur->sh==h2){

			if(firstFound==0){
				firstFound=cur;
			}else{
				temp=cur->sh;
				cur->sh = firstFound->sh;
				firstFound->sh = temp;
			}
		}

		cur=cur->next;
	}
}


void ListHand::copyParentInfo(ListHand* lh){
	blocked = lh->blocked;

	Elem* cur = lh->first;

	while(cur!=0){
		this->add(cur->sh);
		cur=cur->next;
	}

	cntHand = lh->cntHand;

}
