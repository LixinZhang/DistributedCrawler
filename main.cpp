#include<iostream>
#include<stdio.h>
#include<map>
#include<vector>
#include<queue>
#include<errno.h>

#include"urlFrontier/urlfrontier.h"
#include"config.h"
#include"filter/bloom.h"
#include"filter/urlFilter.h"
#include"pthread.h"
#include"fetch/fetchThreadPool.h"
#include"fetch/fetchPage.h"
#include"consistentHash/consistentHash.h"
#include"distributed/communication.h"

/*全局共享区*/

//本机url待采集池
urlfrontier * frontier;
//本机dns缓存
map<string, string> * dnsCache;
//bloom 去重
Bloom * bloom ;
//robotsFilter 缓存
map<string, robotsFilter * > * robotsCache;
//一致性hash
CConHash * conhash;
//client IP -> 待发送url队列 映射
map<string,queue<url *> > clientsMap;
//client IP -> 端口 映射
map<string,int> portMap;
//client IP -> 一致性hash节点 映射
map<string , CNode_s *> CNodes_sMap;
//多client间通信模块
communication * communicator;
//client初始化列表
vector<CNode_s *> clients_Node;
//路由result
map<string,int> route_error_result;

//互斥锁
pthread_mutex_t frontier_mutex;
pthread_mutex_t dnsCache_mutex;
pthread_mutex_t bloom_mutex;
pthread_mutex_t robotsCache_mutex;
pthread_mutex_t clientsMap_mutex;

bool everybody_stop = false;

//线程程序,通过指定的fetcher，绑定到线程进行fetch工作
void * fetch(void * fetcher)
{
	fetchPage * fetch = (fetchPage *) fetcher;
	url * u = NULL;
	while(true)
	{
		if(everybody_stop==true) break;
		pthread_mutex_lock(&frontier_mutex);
		u = frontier->popurl();
		pthread_mutex_unlock(&frontier_mutex);
		if(u==NULL){
			sleep(5); continue;
		}
		while(true){
			timeval now ;
			gettimeofday(&now,NULL);
			if(timercmp(&now,&u->getTe(),>) == true){
				cout<<"#Now seed url is "<<u->giveUrl()
						<<"\t priority is "<<u->getPriority()<<"\tThread:\t"<<pthread_self()<<endl;
				try{
					fetch->fetch(u);
				}catch(exception exce){
					//尽量捕捉异常，但是系统级异常很难捕捉到
					cout<<"Something is wrong\t"<<exce.what()<<endl;
				}
				delete u; break;
			}
			else{
				sleep(1);
			}
		}
	}
	return 0;
}

//线程程序，定时将url路由到其他client
void * router(void * t)
{
	int sleep_t = *((int*)t);//sleep time
	while(true)
	{
		sleep(sleep_t);
		for(map<string,queue<url *> >::iterator iter = clientsMap.begin();iter!=clientsMap.end() ;iter++)
		{
			pthread_mutex_lock(&clientsMap_mutex);
			if(iter->second.size()>0)
			{
				cout<<"向机器 \t"<<iter->first<<"："<<portMap[iter->first]<<"\t发送URL\t"<<iter->second.size()<<endl;
				communicator->send(iter->first,portMap[iter->first],iter->second);
			}
			pthread_mutex_unlock(&clientsMap_mutex);
		}
	}
	return NULL;
}

//线程程序，接收其他client传递过来的url，并加入到本机url frontier中
void * receiver(void * t)
{
	//会一直阻塞在下面
	communicator->recv();
	return NULL;
}

//初始化
void init()
{
	/*
	 * url filter 过滤器的初始化在htmlparser的构造中，而不在这里进行。
	 * 主要因为filter还需要动态更新robotsfilter
	 */

	//初始化互斥锁
	pthread_mutex_init (&frontier_mutex,NULL);
	pthread_mutex_init (&dnsCache_mutex,NULL);
	pthread_mutex_init (&bloom_mutex,NULL);
	pthread_mutex_init (&robotsCache_mutex,NULL);
	pthread_mutex_init (&clientsMap_mutex,NULL);

	//全局UrlFrontier
	frontier = new urlfrontier(FRONTQUEUE_SIZE,BACKQUEUE_SIZE);
	//全局DNS Cache
	dnsCache = new map<string,string>();
	//初始化bloom过滤器，全局去重
	HashFunA *funa = new HashFunA();
	HashFunB * funb = new HashFunB();
	vector<HashFun*>  hashfunclist;
	hashfunclist.push_back(funa);
	hashfunclist.push_back(funb);
	bloom = new Bloom(MAXBLOOMSIZE,hashfunclist);
	//全局robots 过滤器缓存
	robotsCache = new map<string, robotsFilter *>();

	//全局 client间通信模块
	communicator = new communication();

	//是否停止所有work thread
	everybody_stop = false;

	//client_ipaddress->port 映射初始化
#ifdef initClientPortMap
	initClientPortMap(portMap);
#endif

#ifdef initClients
	initClients(clients_Node);
#endif
	if(clients_Node.size()<=0) return;
	CHashFun * func = new CMD5HashFun();
	//初始化一致性hash对象
	conhash = new CConHash(func);
	for(vector<CNode_s *>::iterator iter = clients_Node.begin();iter!=clients_Node.end();iter++)
	{
		conhash->addNode_s(*iter);
		char addr [100];
		strcpy(addr,(char *)((*iter)->getData()));
		CNodes_sMap[(string)addr] = (*iter);
		route_error_result[(string)addr] = 0;
	}

	//初始化 本机urlfrontier
	char seedurl [] = SEEDURL; //种子url
	url * u = new url(seedurl,CRAWLER_DEAPTH,SEEDURL_PRIORITY);
	pthread_mutex_lock(&frontier_mutex);
	frontier->pushurl(u);
	pthread_mutex_unlock(&frontier_mutex);
}

//主程序入口
int main()
{

	init();
	pthread_t router_identifier;
	pthread_t receiver_identifier;
	//启动receiver线程
	pthread_create(&receiver_identifier,NULL,receiver,NULL);
	//启动router线程
	int sleep_time = 5;
	pthread_create(&router_identifier,NULL,router,&sleep_time);
	//启动线程池，进行页面爬取
	fetchThreadPool threadpoll(THREAD_NUMBER,(char *)BASE_NAME);
	threadpoll.start();
	pthread_join(receiver_identifier,NULL);
	pthread_join(router_identifier,NULL);
	//threadpoll.stop();
	return 0;
}
