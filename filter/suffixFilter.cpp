/*
 * suffixFilter.cpp
 *
 *  Created on: 2012-3-26
 *      Author: xiaojay
 */

#include "urlFilter.h"

suffixFilter::suffixFilter()
{
	//Initialization
	suffixlist_size = 0;
	//初始化后缀名过滤列表
#ifdef SUFFIXFILTER
	int pre = 0;
	int pos = 0;
	while(SUFFIXFILTER[pos]!='\0')
	{
		while(SUFFIXFILTER[pos]!='\0'&&SUFFIXFILTER[pos]!='|') pos++;
		strncpy(suffixlist[suffixlist_size],SUFFIXFILTER+pre,pos-pre);
		suffixlist[suffixlist_size][pos-pre] = '\0';
		suffixlist_size++;
		pre = pos+1;
		if(SUFFIXFILTER[pos]!='\0' && suffixlist_size<MAX_SUFFIXFILTER_NUM) pos++;
	}
#endif

}

bool suffixFilter::filter(url * u)
{
	//return true;
	assert(u!=NULL);
	return this->filter(u->getFile());
}

bool suffixFilter::filter(const char * url_text)
{
	//return true;
	assert(url_text!=NULL);
	if(url_text==NULL||strlen(url_text)<=1) return true;
	char suffix [100];
	int len = strlen(url_text);
	int end = len;
	int j;
	for(int i=len-1;i>=0;i--)
	{
		if(end-i>=100) return true;
		if (url_text[i] == '/') return true;
		if (url_text[i] == '?') end = i;
		if (url_text[i] == '.')
		{
			for (j=i;j<end&&j>i;j++)
			{
				suffix[j-i] = url_text[j];
			}
			suffix[j-i] = '\0';
			for (int k=0;k<suffixlist_size;k++)
			{
				if(strcmp(suffixlist[k],suffix)==0)
				{
					return false;
				}
			}
			return true;
		}
	}
	return true;
}

/*
//Test

#include<iostream>
#include<stdio.h>
using namespace std;
int main()
{
	char line [1024];
	urlFilter *filter = new suffixFilter();
	freopen("input.txt","r",stdin);
	for(int i=0;i<1000;i++)
	{
		cin>>line;
		url * u = new url(line,3,3);

		if(!filter->filter(u))
		{
			u->print();
			cout<<endl;
		}

	}
	return 0;
}

*/
