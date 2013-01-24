#ifndef _CMD5HashFun
#define _CMD5HashFun

#include"CMD5HashFun.h"
#include"CHashFun.h"

/*用MD5算法计算结点的hash值，继承CHashFun父类*/
class CMD5HashFun : public CHashFun
{
public:
	virtual long getHashVal (const char * );
};


#endif