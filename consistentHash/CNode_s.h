#ifndef _CNode_s
#define _CNode_s

/*实体结点*/
class CNode_s
{
public:
	/*构造函数*/
	CNode_s();
	CNode_s(char * pIden , int pVNodeCount , void * pData);
	CNode_s(char * pIden , int pVNodeCount , void * pData , int other);

	/*获取结点标示*/
	const char * getIden();

	/*获取实体结点的虚拟结点数量*/
	int getVNodeCount();

	/*设置实体结点数据值*/
	void setData(void * data);

	/*获取实体结点数据值*/
	void * getData();

	inline int getOther(){return other;}
private:
	void setCNode_s(char * pIden, int pVNodeCount , void * pData);
	char iden[100];/*结点标示串*/
	int vNodeCount; /*虚拟结点数目*/
	void * data;/*数据结点*/
	int other;
};
#endif
