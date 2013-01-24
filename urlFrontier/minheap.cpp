/*
 * minheap.cpp
 *
 *  Created on: 2012-3-2
 *      Author: xiaojay
 */

#include"minheap.h"
#include<assert.h>


//constructor
minheap::minheap(int maxheapsize)
{
	assert(maxheapsize>0);
	this->maxheapsize = maxheapsize;
	this->currentsize = 0;
	heap = new node [maxheapsize];
}

//destructor
minheap::~minheap()
{
	delete [] heap;
}

//adjust the heap from top to bottom
void minheap::siftdown(int currentPos , int m)
{
	int i=currentPos;
	int j=currentPos*2+1;//i's leftChild
	timeval temp=heap[i].te;
	int temppos = heap[i].backpos;
	while(j<=m)
	{
		if(j<m&&timercmp(&heap[j].te,&heap[j+1].te,>)) j++; // j points to minChild
		if(timercmp(&temp,&heap[j].te,<=)) break;
		else 
		{
			heap[i].te=heap[j].te;
			heap[i].backpos = heap[j].backpos;
			i=j;
			j=2*i+1;
		}
	}
	heap[i].te=temp;
	heap[i].backpos = temppos;
}

//adjust the heap from bottom to top
void minheap::siftup(int start)
{
	int i=start,j=(i-1)/2;
	timeval temp=heap[i].te;
	int temppos = heap[i].backpos;
	while(i>0)
	{
		if(timercmp(&heap[j].te,&temp,>))
		{
			heap[i].te=heap[j].te;
			heap[i].backpos = heap[j].backpos;
			i=j;
			j=(i-1)/2;
		}
		else break;
	}
	heap[i].te=temp;
	heap[i].backpos = temppos;
}

//add a node to heap 
void minheap::insert(timeval te , int backpos)
{
	if(currentsize>=maxheapsize)
	{
		return ;
	}
	heap[currentsize].te=te;
	heap[currentsize].backpos = backpos;
	siftup(currentsize);
	currentsize++;
}

//pass value to backpos and te then remove the node 
void minheap::removemin(int & backpos , timeval & te)
{
	assert(currentsize>=0);
	backpos = heap[0].backpos;
	te = heap[0].te;
	heap[0] = heap[currentsize-1];
	currentsize --;
	siftdown(0, currentsize-1);
}


/*Test min heap*/
/*
#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	cout<<"Start from here ..."<<endl;
	minheap heap(100);
	const int size=10;
	timeval arr[size];
	for(int i=0;i<size;i++)
	{
		gettimeofday(&arr[i],NULL);

	}
	int p[] = {2,1,3,0,8,6,9,7,4,5};

	for(int i=0;i<size;i++)
		heap.insert(arr[p[i]],i+1);

	timeval t ;
	int pos;
	for(int i=0;i<size;i++)
	{
		heap.removemin(pos,t);
		printf("---Pos: %d \t ---time: %d \n",pos,t.tv_usec);
	}
	return 0;
}

*/
