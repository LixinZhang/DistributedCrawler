#include"CConHash.h"
#include"assert.h"
#include"CVirtualNode_s.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>

CConHash::CConHash(CHashFun * pFunc)
{
	/*设置hash函数*/
	assert(pFunc!=NULL);
	this->func = pFunc;
	this->vNodes = 0;
	/*初始化红黑树*/
	vnode_tree = new util_rbtree_s();
	util_rbtree_init(vnode_tree);
}

int CConHash::addNode_s(CNode_s * pNode)
{
	if(pNode==NULL) return -1;
	int vCount = pNode->getVNodeCount();
	if(vCount<=0) return -1;
	CVirtualNode_s * virtualNode ;
	util_rbtree_node_t * rbNode;
	char str [1000];
	char num[10];
	strcpy(str,pNode->getIden());
	long hash = 0;
	/*生成虚拟结点并插入到红黑树中*/
	for(int i=0;i<vCount;i++)
	{
		virtualNode = new CVirtualNode_s(pNode);
		/*采用str+“i”的方法产生不同的iden串，用于后面的hash值计算*/
		sprintf(num,"%d",i);
		strcat(str,num);
		hash = func->getHashVal(str);
		virtualNode->setHash(hash);
		if(!util_rbtree_search(vnode_tree,hash))
		{
			/*生成红黑树结点*/
			rbNode = vNode2RBNode(virtualNode); 
			if(rbNode!=NULL)
			{
				/*将该结点插入到红黑树中*/
				util_rbtree_insert(vnode_tree,rbNode);
				this->vNodes++;
			}
		}
	}
	return 0;
}

int CConHash::delNode_s(CNode_s * pNode)
{
	if(pNode==NULL) return -1;
	util_rbtree_node_t * rbNode;
	char str [1000];
	char num [10];
	strcpy(str,pNode->getIden()); 
	int vCount = pNode->getVNodeCount();
	long hash = 0;
	CVirtualNode_s * node = NULL;
	/*将该实体结点产生的所有虚拟结点进行删除*/
	for(int i=0;i<vCount;i++)
	{
		sprintf(num,"%d",i);
		strcat(str,num);/*采用该方法产生不同的iden串*/
		hash = func->getHashVal(str);
		rbNode = util_rbtree_search(vnode_tree,hash);
		if(rbNode!=NULL)
		{
			node = (CVirtualNode_s *) rbNode->data;
			if(node->getNode_s()==pNode && node->getHash()==hash)
			{
				this->vNodes--;
				/*将该结点从红黑树中删除*/
				util_rbtree_delete(vnode_tree,rbNode);
				delete rbNode;
				delete node;
			}
		}
	}
	return 0;
}

CNode_s * CConHash::lookupNode_s(const char * object)
{
	if(object==NULL||this->vNodes==0) return NULL;
	util_rbtree_node_t * rbNode;
	int key = this->func->getHashVal(object);
	/*在红黑树中查找key值比key大的最小的结点*/
	rbNode = util_rbtree_lookup(vnode_tree,key);
	if(rbNode!=NULL)
	{
		return ((CVirtualNode_s *) rbNode->data)->getNode_s();
	}
	return NULL;
}

int CConHash::getVNodes()
{
	return this->vNodes;
}


util_rbtree_node_t * vNode2RBNode(CVirtualNode_s * vnode)
{
	if(vnode==NULL) return NULL;
	util_rbtree_node_t *rbNode = new util_rbtree_node_t(); 
	rbNode->key = vnode->getHash();
	rbNode->data = vnode;
	return rbNode;
}	


