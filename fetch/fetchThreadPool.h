/*
 * threadPool.h
 * A very simple thread pool
 *  Created on: Apr 13, 2012
 *      Author: mayday
 */

#ifndef THREADPOOL_H_
#define THREADPOOL_H_

#include<pthread.h>
#include<stdio.h>
#include"../config.h"
#include"fetchPage.h"

extern void * fetch(void *);
extern bool everybody_stop;

class fetchThreadPool{
public:
	fetchThreadPool(int threadNum , char * basename);
	//~fetchThreadPool();
	void start();
	void stop();
private:
	pthread_t Idlist [MAX_THREAD_NUMBER];
	int threadNumber;
	char basename [100];
};

#endif /* THREADPOOL_H_ */
