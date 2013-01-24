/*
 * urlFilter.h
 *
 *  Created on: 2012-3-26
 *      Author: xiaojay
 */

#ifndef URLFILTER_H_
#define URLFILTER_H_
#include "../urlFrontier/url.h"
#include "../config.h"
#include <string.h>
#include <assert.h>

//接口
class urlFilter
{
public:
	// 当u合法时，返回true，否则返回false
	virtual bool filter(url * u) = 0;
	virtual bool filter(const char * url_text) = 0;
};


//主机名过滤
class hostFilter : public urlFilter
{
public:
	hostFilter();
	virtual bool filter(url * u);
	virtual bool filter(const char * url_text);
private:
	char hostlist [MAX_HOSTFILTER_NUM][100];
	int hostlist_size;
};

//后缀名过滤
class suffixFilter: public urlFilter
{
public:
	suffixFilter();
	//return true if url's suffix is not in suffixlist , otherwise return false
	virtual bool filter(url * u);
	virtual bool filter(const char * url_text);
private:
	char suffixlist [MAX_SUFFIXFILTER_NUM][100];
	int suffixlist_size ;
};

//robots.txt过滤
class robotsFilter : public urlFilter
{
public:
	robotsFilter(const char * host_name , const char * robots_txt);
	~robotsFilter();
	virtual bool filter(url * u);
	virtual bool filter(const char * url_text);
	void parseRobots();
	void printDisallow();
	bool robotsMatch(char * match , char * file);
private:
	char * strlwr(char *); //辅助函数，大写转小写
	char * disallow [MAXDISALLOW];
	int disallow_size;
	char * allow[MAXDISALLOW];
	int allow_size;
	char * hostname;
	char * robotstxt;
	int crawldelay ;
};

#endif /* URLFILTER_H_ */
