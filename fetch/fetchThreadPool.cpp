/*
 * fetchThreadPool.cpp
 *
 *  Created on: Apr 13, 2012
 *      Author: mayday
 */

#include "fetchThreadPool.h"

fetchThreadPool::fetchThreadPool(int threadNum , char * basename)
{
	assert(threadNum<=MAX_THREAD_NUMBER);
	this->threadNumber = threadNum;
	strcpy(this->basename, basename);
}


void fetchThreadPool::start()
{
	char currentfilename [100];
	for(int i=0;i<threadNumber;i++){
		char num_str [7];
		sprintf(num_str,"%03d_",i+1);
		strcpy(currentfilename,basename);
		strcat(currentfilename,num_str);
		fetchPage * fetcher = new fetchPage(currentfilename);
		int res = pthread_create(&(this->Idlist[i]),NULL,fetch,fetcher);
		if (res!=0) {
			cout<<"Some thing is wrong when create threads."<<endl;
		}
	}
	for(int i=0;i<threadNumber;i++)
		pthread_join(this->Idlist[i],NULL);
}

void fetchThreadPool::stop()
{
	everybody_stop = true;
}

