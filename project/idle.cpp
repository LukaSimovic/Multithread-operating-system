/*
 * idle.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */


#include"idle.h"
#include"thread.h"
#include"pcb.h"
#include<iostream.h>
#include"settings.h"
#include<stdio.h>


void Idle::run(){
	while(1){
		/*
		lock
		printf("idle!");
		unlock
		*/
	}
}

PCB* Idle::getIdlePCB(){
	return this->myPCB;
}

