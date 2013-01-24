/*
 * minheap.h
 *
 *  Created on: 2012-2-24
 *      Author: xiaojay
 */

#ifndef MINHEAP_H_
#define MINHEAP_H_
#include <sys/time.h>

struct node {
	timeval te;
	int backpos;
public:
	node() //default constructor
	{
		backpos = -1;
	}
};

class minheap 
{
private:
	int maxheapsize;
	int currentsize;
	node * heap;
	//adjust the head from upon downto bottom
	void siftdown(int currentPos , int m);
	//adjust the heap from bottom to upon
	void siftup(int start);
public:
	//build a min heap
	minheap(int maxheapsize);
	~minheap();
	//insert an element
	void insert(timeval te, int backpos);
	//remove an element
	//position of back queue in urlfrontier.h and node.te returns as reference value
	void removemin(int & backpos , timeval & te);
	inline int size()  {return this->currentsize ;}
};


#endif
