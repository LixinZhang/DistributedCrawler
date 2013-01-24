/*
 * urlfrontier.cpp
 *
 *  Created on: 2012-3-2
 *      Author: xiaojay
 */

#include "urlfrontier.h"


//constructor
urlfrontier::urlfrontier( int nf , int nb)
{
	this->maxF = nf;
	this->maxB = nb;
	this->size = 0;
	frontQueue = new queue<url *>[maxF];
	backQueue  = new queue<url *>[maxB];
	heap = new minheap(nb);
}

//destructor
urlfrontier::~urlfrontier()
{
	delete [] frontQueue;
	delete [] backQueue;
	delete heap;
}

//add url 
void urlfrontier::pushurl(url * u)
{
	if(!u->isValid()) return ;
	int priority = u->getPriority();
	if(priority>=maxF|| priority<0) return ;
	frontQueue[priority].push(u);
	size++;
	//router();
}

//route urls from frontQueue to backQueue
bool urlfrontier ::router()
{
	int pos = maxF;
	int size = 0;
	/*MAX_URLS_ONCE limits the max number of urls moved ,
	to ignore much expenses*/
	int limit = MAX_URLS_ONCE;
	while(pos>0&&limit>0)
	{
		limit--;
		pos--;
		size = frontQueue[pos].size();
		if (size==0) continue;
		while(!frontQueue[pos].empty()&&size>0)
		{
			size--;
			url * u = frontQueue[pos].front();
			if(hostmap.count(u->getHost()))
			{
				int backpos = hostmap[u->getHost()];
				backQueue[backpos].push(u);
				frontQueue[pos].pop();
			}
			else
			{
				//find empty pos ;
				int posB = 0;
				while(posB<maxB&&!backQueue[posB].empty()) posB++;
				if(posB==maxB) 	
				{
					frontQueue[pos].pop();
					frontQueue[pos].push(u);
					continue;
				}
				//update the hostmap
				hostmap.insert(map<string,int>::value_type(u->getHost(),posB));
				backQueue[posB].push(u);
				//update heap
				timeval now ;
				gettimeofday(&now,NULL);
				heap->insert(now,posB);
				frontQueue[pos].pop();
			}
		}
	}
	return true;
}

//get a url
url * urlfrontier::popurl()
{
	if(heap->size()<=0)
	{
		router();
	}
	if(heap->size()<=0) return NULL;
	int backpos;
	timeval te;
	heap->removemin(backpos,te);
	url * u = backQueue[backpos].front();
	u->setTe(te);
	if(u==NULL) return NULL;
	backQueue[backpos].pop();
	if(backQueue[backpos].empty())
	{
		router();//route urls from frontqueue to backqueue
		hostmap.erase(u->getHost());
	}
	else
	{
		//the time between two request is 5 seconds
		te.tv_sec += 5;
		heap->insert(te,backpos);
	}
	size --;
	return u ;
}

/*
//display the frontQueue and backQueue
// but it will pop(), so it must be used in the end 
void urlfrontier::displayState()
{
	router();
	int i =0 ;	
	for(i=0;i<maxF;i++)
	{
		while(!frontQueue[i].empty())
		{
			printf("frontqueue: %d \t host  :%s\n", i,frontQueue[i].front()->giveUrl());
			frontQueue[i].pop();
		}
	}
	for(i=0;i<maxB;i++)
	{
		while(!backQueue[i].empty())
		{
			printf("frontqueue: %d \t host  :%s\n", i,backQueue[i].front()->giveUrl());
			backQueue[i].pop();
		}
	}
}

*/
