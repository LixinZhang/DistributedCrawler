#include"bloom.h"

Bloom::Bloom(int size , std::vector<HashFun*> hashfunclist)
{
	assert(hashfunclist.size()>0);
	this->size = size;
	this->hashfunclist = hashfunclist;
	this->arr = new char [size];
	this->count = 0;
}

Bloom::~Bloom()
{
	if(this->arr!=NULL)
	{
		delete this->arr;
	}
}

void Bloom::add(const char * text)
{
	int nfunc = hashfunclist.size();
	long code = 0;
	for(int i=0;i<nfunc;i++)
	{
		code = hashfunclist.at(i)->gethashval(text);
		code = code % (size * CHARBITSIZE);
		if(code/CHARBITSIZE>size) return;
		else
		{
			setbit(code);
		}
	}
	count++;
}

bool Bloom::check(const char * text)
{
	bool res = true;
	int nfunc = hashfunclist.size();
	long code = 0;
	for (int i=0;i<nfunc;i++)
	{
		code = hashfunclist.at(i)->gethashval(text);
		code = code % (size * CHARBITSIZE) ;
		if(code/CHARBITSIZE>size) return false;
		else
		{
			res &= getbit(code);
			if (res==false) return res;
		}
	}
	return true;
}

inline void Bloom::setbit(long code)
{
	arr[code/CHARBITSIZE] |= (1<<(code%CHARBITSIZE));
}

inline bool Bloom::getbit(long code)
{
	if(!(arr[code/CHARBITSIZE] & (1<<(code%CHARBITSIZE))))
	{
		return false;
	}
	return true;
}
