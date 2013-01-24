/*
 * urlfrontier.h
 *
 *  Created on: 2012-3-2
 *      Author: xiaojay
 */

#ifndef URLFRONTIER_H_
#define URLFRONTIER_H_
#include <queue>
#include <map>
#include <string>
#include "url.h"
#include "minheap.h"
#include "../config.h"
#include <time.h>
#include <sys/time.h>

class urlfrontier
{
private:
	/*frontQueue maintains urls which have higher priority 
	  cound route to back queue more quickly.
	frontQueue[i] contains all urls whose priority is i.*/
	queue<url *> * frontQueue;
	/*
	  backQueue[i] contains urls whose have the same hostname 
	*/
	queue<url *> * backQueue;
	int maxF,maxB;//the max number of frontQueue and backQueue
	map<string,int> hostmap;
	minheap * heap;
	//route urls from frontQueue to backQueue
	bool router();
public:
	//constructor
	//parameters: nf->maxF, nb->maxB
	urlfrontier (int nf , int nb);
	//destructor
	~urlfrontier();
	//add a url
	void pushurl(url * u) ;
	//get a url
	url * popurl();
	void displayState();
	int size;
	int getsize(){return this->size;}
};

#endif /* URLFRONTIER_H_ */
