/*
 * hostFilter.cpp
 *
 *  Created on: 2012-3-27
 *      Author: xiaojay
 */

#include "urlFilter.h"

hostFilter::hostFilter()
{
	hostlist_size = 0;
//初始化host过滤列表
#ifdef HOSTFILTER
	int pre = 0;
	int pos = 0;
	while(HOSTFILTER[pos]!='\0')
	{
		while(HOSTFILTER[pos]!='\0'&&HOSTFILTER[pos]!='|') pos++;
		strncpy(hostlist[hostlist_size],HOSTFILTER+pre,pos-pre);
		hostlist[hostlist_size][pos-pre] = '\0';
		hostlist_size++;
		pre = pos+1;
		if(HOSTFILTER[pos]!='\0' && hostlist_size<MAX_HOSTFILTER_NUM) pos++;
	}
#endif
}

bool hostFilter::filter(url * u)
{
	//return true;
	assert(u!=NULL);
	return filter(u->getHost());
}

bool hostFilter::filter(const char * url_text)
{
	//return true;
	if(url_text==NULL||strlen(url_text)<=1) return true;
	char hostname [100];
	int pos1 = 0;
	int pos2 = 0;
	int len = strlen(url_text);
	while(pos1<len
			&&(url_text[pos1]!='/'||url_text[pos1+1]!='/')) pos1++;
	pos1+=2;
	pos2 = pos1;
	while(pos2<len
			&&(url_text[pos2]!='/'&&url_text[pos2]!=':')) pos2++;
	for (int i=pos1;i<pos2;i++)
	{
		hostname[i-pos1] = url_text[i];
	}
	hostname[pos2-pos1] = '\0';

	for(int i=0;i<hostlist_size;i++)
	{
		if(strcmp(hostlist[i],hostname)==0)
		{
			return false;
		}
	}

	return true;
}


/*
//test

#include<iostream>
using namespace std;
int main()
{
	urlFilter * filter = new hostFilter();
	cout<<filter->filter("http://www.baiddu.com:80")<<endl;
	return 0;
}

*/
