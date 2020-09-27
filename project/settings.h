/*
 * settings.h
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */

#ifndef SETTINGS_H_
#define SETTINGS_H_

class PCB;
class Thread;
class Idle;

#define lock asm pushf; asm cli; {}
#define unlock asm popf; {}


class Settings{
public:
	static Thread* mainThread;
	static Idle* idleThread;
	static void init();
	static void restore();
	static void createMainPCB();

	static volatile unsigned int lockFlag;

	static void killThread();

};

void interrupt timer();
void exitThread();

#endif /* SETTINGS_H_ */
