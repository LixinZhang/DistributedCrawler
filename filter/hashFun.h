/*
 * hashFun.h
 *
 *  Created on: 2012-2-22
 *      Author: xiaojay
 */

#ifndef HASHFUN_H_
#define HASHFUN_H_
#include<stdlib.h>

class HashFun
{
public :
	virtual long gethashval(const char * key) = 0;
};

class HashFunA : public HashFun
{
public:
	virtual long gethashval(const char * key)
	{
		unsigned int h=0;
		while(*key) h^=(h<<5)+(h>>2)+(unsigned char)*key++;
		return abs(h);
	}
};

class HashFunB : public HashFun
{
public:
	virtual long gethashval(const char * key)
	{
		unsigned int h=0;
		while(*key) h=(unsigned char)*key++ + (h<<6) + (h<<16) - h;
		return abs(h);
	}
};


#endif /* HASHFUN_H_ */
