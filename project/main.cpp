/*
 * main.cpp
 *
 *  Created on: Aug 12, 2020
 *      Author: OS1
 */


#include"settings.h"

extern int userMain(int argc, char* argv[]);

int main(int argc, char* argv[]){
	Settings::init();
	int ret = userMain(argc, argv);
	Settings::restore();
	return ret;
}

