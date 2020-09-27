/*
 * kerSem.h
 *
 *  Created on: Aug 13, 2020
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

#include"semaphor.h"
#include<iostream.h>


class ListPCB;
class ListSem;

class KernelSem{
	public:
		KernelSem(int init);

		int wait(Time mTime);
		int signal(int n);

		static ListSem* allSemList;

		ListPCB* blockedList;

		void updateVal(int t);
		int getVal();

		~KernelSem();

		friend ostream& operator<<(ostream& os, KernelSem* k){
				os<<"Semaphore. value: "<<k->getVal()<<endl;
				return os;
		}

	private:
		int val;
};


#endif /* KERSEM_H_ */
