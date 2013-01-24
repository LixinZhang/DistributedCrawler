/*
 * bloom.h
 *
 *  Created on: 2012-2-22
 *      Author: xiaojay
 */

#ifndef BLOOM_H_
#define BLOOM_H_
#include <vector>
#include "hashFun.h"
#include "../config.h"

class Bloom
{
public :
	Bloom(int size , std::vector<HashFun*> hashfunclist);
	~Bloom();
	void add(const char * text);
	bool check(const char * text);
	inline int elementCount(){return count;}
private :
	const static long CHARBITSIZE = 8;
	long size;
	char * arr;
	int count;
	std::vector<HashFun*> hashfunclist;
	inline void setbit(long pos);
	inline bool getbit(long pos);
};

#endif /* BLOOM_H_ */
