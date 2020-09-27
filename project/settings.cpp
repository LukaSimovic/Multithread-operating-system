/*
 * settings.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */


#include"settings.h"
#include"pcb.h"
#include"thread.h"
#include"settings.h"
#include"idle.h"
#include"SCHEDULE.h"
#include<stdio.h>
#include"kerSem.h"
#include"listSem.h"
#include"listPCB.h"
#include"listHand.h"

unsigned oldTimerOFF, oldTimerSEG;

Thread* Settings::mainThread=0;
Idle* Settings::idleThread=0;

volatile unsigned int Settings::lockFlag=0;


extern void tick();

void Settings::init(){
	lock
	asm{
		cli
		push es
		push ax
		mov ax,0
		mov es,ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer

		mov ax, oldTimerSEG

		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}



	createMainPCB();
	unlock
}


void Settings::createMainPCB(){
	mainThread=new Thread();

	PCB::running=mainThread->myPCB;
	PCB::running->state=PCB::READY;

	idleThread = new Idle();
	idleThread->start();

}

void Settings::restore(){
	lock
	asm{
		cli
		push es
		push ax
		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}


	delete idleThread;
	delete mainThread;

	delete PCB::allPCBlist;
	delete KernelSem::allSemList;
	unlock
}



volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;
volatile int cntr= defaultTimeSlice; //2
volatile int context_switch_on_demand=0;


void interrupt timer(){

	if(!context_switch_on_demand){
		if(PCB::running->quantum!=0){
			cntr--;
		}
		asm int 60h;
		tick();
		KernelSem::allSemList->updateTimeList();
	}


	if((cntr==0 && PCB::running->quantum!=0) || context_switch_on_demand){
		if(Settings::lockFlag==0){
			context_switch_on_demand=0;
			asm{
				mov tsp,sp
				mov tss,ss
				mov tbp,bp
			}
			PCB::running->sp=tsp;
			PCB::running->ss=tss;
			PCB::running->bp=tbp;

			if(PCB::running->state==PCB::READY && PCB::running != Settings::idleThread->getIdlePCB()){
				Scheduler::put((PCB*)PCB::running);
			}

			PCB::running= Scheduler::get();

			if(PCB::running==0){
				PCB::running = Settings::idleThread->getIdlePCB();
			}

			if(PCB::running!=Settings::idleThread->getIdlePCB()){
						((PCB*)PCB::running)->handleSignals();
			}


			tsp = PCB::running->sp;
			tss=PCB::running->ss;
			tbp=PCB::running->bp;
			cntr=PCB::running->quantum;

			asm{
				mov sp, tsp
				mov ss, tss
				mov bp, tbp
			}


		}else{
			context_switch_on_demand=1;
		}
	}

}

void dispatch(){ //deklarisana u Thread
	lock
		context_switch_on_demand=1;
		timer();
	unlock
}

void exitThread(){
	lock
		PCB::running->waitingList->unblockWaitingList();
		PCB::running->state=PCB::FINISHED;
	unlock
	dispatch();
}


void Settings::killThread(){

	PCB::running->wrapperFlag=1;
	PCB::sendSystemSignals();
	PCB::running->wrapperFlag=0;

	PCB::running->waitingList->unblockWaitingList();
	PCB::running->state=PCB::FINISHED;


}
