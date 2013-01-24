#include"CVirtualNode_s.h"
#include<iostream>
#include<assert.h>
CVirtualNode_s::CVirtualNode_s()
{
	node = NULL;
}

CVirtualNode_s::CVirtualNode_s(CNode_s * pNode)
{
	setNode_s(pNode);
}

void CVirtualNode_s::setNode_s(CNode_s * pNode)
{
	assert(pNode!=NULL);
	node = pNode;
}

CNode_s * CVirtualNode_s::getNode_s()
{
	return node;
}

void CVirtualNode_s::setHash(long pHash)
{
	hash = pHash;
}

long CVirtualNode_s ::getHash()
{
	return hash;
}