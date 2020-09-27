/*
 * ListHand.h
 *
 *  Created on: Aug 20, 2020
 *      Author: OS1
 */

#ifndef ListHand_H_
#define ListHand_H_

#include"thread.h"



class ListHand{
public:
	ListHand();
	~ListHand();

	int blocked;

	void add(SignalHandler s);
	void callAllHandlers();
	void unregisterHandlers();
	void swap(SignalHandler h1, SignalHandler h2);
	void copyParentInfo(ListHand* lh);


	//ispis liste hendlera za jedan signal
	friend ostream& operator<<(ostream& os, ListHand* lh){

		Elem* cur = lh->first;
		if(lh->cntHand==0)
			os<<"List is empty!"<<endl;
		else{
			os<<"Size: "<<lh->cntHand<<endl;
			os<<"Blocked: "<<lh->blocked<<endl;
			while(cur){
				//(*cur->sh)();
				cur=cur->next;
			}
		}

		return os;

	}



private:

	struct Elem{
		SignalHandler sh;
		Elem* next;

		Elem(SignalHandler s){
			sh=s;
			next=0;
		}
	};

	Elem *first,*last;
	int cntHand;
};


#endif /* ListHand_H_ */
