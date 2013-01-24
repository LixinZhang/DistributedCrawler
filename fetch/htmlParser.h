/*
 * htmlParser.h
 *  解析HTML && HTTP协议 && url 提取、去重、过滤
 *  Created on: Apr 6, 2012
 *      Author: mayday
 */

#ifndef HTMLPARSER_H_
#define HTMLPARSER_H_

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <queue>
#include <map>

#include "../urlFrontier/url.h"
#include "../urlFrontier/urlfrontier.h"
#include "../filter/bloom.h"
#include "../filter/urlFilter.h"
#include "../config.h"
#include "../consistentHash/consistentHash.h"

extern Bloom * bloom;
extern urlfrontier * frontier;

extern pthread_mutex_t frontier_mutex;
extern pthread_mutex_t bloom_mutex;
extern pthread_mutex_t clientsMap_mutex;

extern CConHash * conhash;
extern map<string,queue<url *> > clientsMap;

class htmlParser
{
public:
	htmlParser();
	//解析HTML HEADER
	int parseHeader(const char * head ,unsigned int len );
	//解析Location重定向
	url * parseLocation(const char * head , unsigned int len , url * u);
	//抽取HREF，并存入到本地队列以及外部URL池中
	void parseHref(const char * html , url * u ,
			queue<url *> & localQueue);
	//去重处理，采用bloom filter
	bool urlseen(url *);
	//url去重
	bool urlfilter(url *);
	//过滤器列表
	vector<urlFilter *> filterlist;
	//优先级 打分
	int getScore(url * ,bool );
};

#endif /* HTMLPARSER_H_ */
