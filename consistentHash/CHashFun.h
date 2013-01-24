#ifndef _CHashFun
#define _CHashFun

/*定义Hash函数类接口，用于计算结点的hash值*/

class CHashFun
{
public:
	virtual long getHashVal(const char *) = 0;
};

#endif