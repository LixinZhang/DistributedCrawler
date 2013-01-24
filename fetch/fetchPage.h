/*
 * fetchPage.h
 * 核心模块，页面抓取，网络通信，HTTP处理
 *  Created on: 2012-3-25
 *      Author: zhanglixinseu@gmail.com
 */

#ifndef FETCHPAGE_H_
#define FETCHPAGE_H_
#include "../urlFrontier/url.h"
#include "../urlFrontier/urlfrontier.h"
#include "indexFile.h"
#include "../filter/urlFilter.h"
#include "../config.h"

#include <map>
#include <string>
#include <assert.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <netdb.h>
#include <sys/time.h>
#include <fcntl.h>
#include <stdlib.h>
#include <queue>
#include <pthread.h>

#include "htmlParser.h"

extern urlfrontier * frontier;
extern map<string,string> * dnsCache;
extern map<string,robotsFilter *> * robotsCache;

extern pthread_mutex_t frontier_mutex;
extern pthread_mutex_t dnsCache_mutex;
extern pthread_mutex_t robotsCache_mutex;


class fetchPage
{
public:
	fetchPage(const char * basename);
	~fetchPage();
	//抓取u指定页面
	void fetch(url * u);
	//fetch robots.txt文件，并返回robots过滤器
	robotsFilter * fetchRobots(url * u);
	//建立socket连接，返回socket id
	int buildConnection(url * u);
	//一次网络交互
	bool communication(url * u , int trytimes);
	//打印
	int statisticsCount(const int code);
	void displayStatistics();
	void displayDnsCache();

	queue<url *> localQueue;
private :
	// 设置IP地址
	void sethostaddr(url * u);
	// key : code , value: counts of the code
	map<int ,int> result;
	//索引存储文件
	indexFile * indexfile;
	//html 解析对象
	htmlParser parser;
	//文件模式
	mode_t mode;

	//文件描述字
	int filefd;
	//套接字
	int sockfd;
	//文件基础名，用于存储页面文件以及索引文件的命名
	//存储文件命名:filebasename000000——filebasename999999
	//索引文件命名:filebasename.index
	char * filebasename;
	//当前存储文件名
	char currentfilename [32];
	//一些统计参数
	int currentcounter;
	int filecounter;
	int fetchedcounter;

	//char recvbuf [MAXLINE];
};

#endif /* FETCHPAGE_H_ */
