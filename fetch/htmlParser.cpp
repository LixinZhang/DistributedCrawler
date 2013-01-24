/*
 * htmlParser.cpp
 *
 *  Created on: Apr 6, 2012
 *      Author: mayday
 */

#include "htmlParser.h"

htmlParser::htmlParser()
{
	//初始化过滤器
	filterlist.push_back(new hostFilter());
	filterlist.push_back(new suffixFilter());
	srand(time(0));
}

int htmlParser::parseHeader(const char * head ,unsigned int len )
{
	assert(head!=NULL);
	//len = strlen(head)<len?strlen(head):len;
	int code = 0;
	unsigned int pos = 0;
	while(pos<len&&head[pos]!=' ') pos++;
	unsigned int pos2 = pos+1;
	while(pos2<len&&head[pos2]!=' ') pos2++;
	for (unsigned int i=pos+1;i<pos2;i++)
	{
		code = code *10 + (head[i]-'0');
	}
	return code;
}

url * htmlParser::parseLocation (const char * head ,unsigned int len , url * u)
{
	assert(head!=NULL);
	//len = strlen(head)<len?strlen(head):len;
	unsigned int pos=0,pos2=0, pos3=0;
	char key[48];
	while(pos<len&&pos2<len&&pos3<len)
	{
		while(pos<len&&head[pos]!='\n') pos++;
		pos2 = pos+1;
		while(pos2<len&&head[pos2]!=':') pos2++;
		pos3 = pos2+1;
		while(pos3<len&&head[pos3]!='\n') pos3++;
		pos++;
		for(unsigned int i=pos;i<pos2;i++) key[i-pos]=head[i];
		key[pos2-pos] = '\0';
		if(strcmp(LOCATION,key)==0)
		{
			pos2+=2;
			char * location = new char [pos3-pos2];
			for(unsigned int k=pos2;k<pos3;k++)
			{
				location[k-pos2] = head[k];
			}
			location[pos3-pos2-1] = '\0';
			url * uu ;
			if(location[0]=='h'&&location[1]=='t'&&location[2]=='t'&&location[3]=='p')
			{
				uu = new url(location,u->getDepth(),u->getPriority());
			}
			else
			{
				uu = new url(u->getHost(),80,location,u->getDepth(),u->getPriority());
			}

			delete location;
			return uu;
		}
		pos=pos3;
	}
	return NULL;
}

bool htmlParser::urlseen(url * u)
{
	char * url_text = u->giveUrl();
	pthread_mutex_lock(&bloom_mutex);
	bool res = bloom->check(url_text);
	pthread_mutex_unlock(&bloom_mutex);
	if(res) return true;
	pthread_mutex_lock(&bloom_mutex);
	bloom->add(url_text);
	pthread_mutex_unlock(&bloom_mutex);
	return false;
}

bool htmlParser::urlfilter(url * u)
{
	for(vector<urlFilter *>::iterator iter = filterlist.begin() ; iter!=filterlist.end();iter++)
	{
		if((*iter)->filter(u)==false) return false;
	}
	return true;
}

void htmlParser::parseHref
	(const char * html , url * u ,queue<url *> & localQueue)
{
	const char *p=html;
	const char *p1=NULL;
	const char *p2="m";
	const char *aneedle="<a";
	const char *mneedle="<map";
	const char *hneedle="href";
	const char *start=0;
	int len;
	char href[BUFSIZ];
	if(!p||!*p) return;
	while(*p)
	{
	   p1=strstr(p,aneedle);
	   if(p2) p2=strstr(p,mneedle);
	   if(!p2&&p1) p=p1;
	   else if(!p1&&!p2) return;
	   else if(!p1&&p2) p=p2;
	   else p=p1>p2?p2:p1;
	   p=strstr(p,hneedle);
	   if(!p) return;
	   while(*p&&*p!='='&&*p!='>') p++;   //find '='
	   if(*p++=='=')
	   {
		   while (*p&&(*p==' '||*p=='\"'||*p=='\'')) p++; //find start
		   if(*p&&*p!='>'){
			 start=p;
			 len=0;
			 while (*p&&*p!=' '&&*p!='\"'&&*p!='\''&&*p!='>'){
				 p++; len++;
			 } //find end
			 if(*p){
			  memset(href,0,BUFSIZ);
			  strncpy(href,start,len);

			  //处理 # 相关
			  if(href[0]=='#') continue;
			  int l = strlen(href);
			  int k = l-1;
			  while(k>=0 && href[k]!='#') k--;
			  if(k>=0){
				  href[k] = '\0';
				  l = k;
			  }

			  url * uu = NULL;
			  char url_text [1024];
			  //这里还要处理下复杂路径的混合搭配
			  //处理非http开头的href，内链
			  if(strstr(href,"http")!=href){
				  if(l>0&&href[0]=='/'){
					  strcpy(url_text,u->getHost());
					  strcat(url_text,href);
				  }
				  else if(l>1&&href[0]=='.'&&href[1]=='/'){
					  strcpy(url_text,u->getRootPath());
					  strcat(url_text,href+1);
				  }
				  else if(l>2&&href[0]=='.'&&href[1]=='.'&&href[2]=='/'){
					  strcpy(url_text,u->getHost());
					  strcat(url_text,href+2);
				  }
				  else{
					  strcpy(url_text,u->getRootPath());
					  strcat(url_text,"/");
					  strcat(url_text,href);
				  }
				  //深度-1
				  if(u->getDepth()>0)
				 	uu = new url(url_text,u->getDepth()-1,getScore(u,true));
			  }
			  else{
				  //外链
				  if(u->getDepth()>0)
					  uu = new url(href,CRAWLER_DEAPTH ,getScore(u,false));
			  }
			  //对url进行过滤和检查： 合法性、去重、过滤
			  if(uu!=NULL&&uu->isValid()&&!urlseen(uu)&&urlfilter(uu))
			  {
				  if(strcmp(u->getHost(),uu->getHost())==0){
					  //加入到本地队列
					  localQueue.push(uu);
				  }
				  else{
					  //通过host主机名，进行分布式划分
					  CNode_s * node = conhash->lookupNode_s(uu->getHost());
					  //如果结果节点属于本机，则直接添加到URL解析池中
					  if(strcmp(node->getIden(),MACHINE_NAME)==0){
						  pthread_mutex_lock(&frontier_mutex);
						  frontier->pushurl(uu);
						  pthread_mutex_unlock(&frontier_mutex);
					  }
					  else
					  {
						  //如果结果节点不属于本机，添加到<主机-URL队列>映射表
						  pthread_mutex_lock(&clientsMap_mutex);
						  char ipaddr [50];
						  strcpy(ipaddr,(char *)node->getData());
						  clientsMap[string(ipaddr)].push(uu);
						  pthread_mutex_unlock(&clientsMap_mutex);
					  }
				  }
			  }
			 }
			}
	   }
	}
}

#include<time.h>
#include<stdlib.h>
// 打分，未完善
int htmlParser::getScore(url * u , bool inner)
{
	int score = u->getPriority();
	if(score<=0) return 0;
	if(inner){
		return score - 1;
	}
	else{
		return score;
	}
}

