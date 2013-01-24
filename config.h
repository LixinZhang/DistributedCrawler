/*
 * config.h
 * Web crawler的所有初始化参数、静态常量配置
 *  Created on: Apr 7, 2012
 *      Author: mayday
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include<iostream>
#include<assert.h>
#include<string.h>
using namespace std;

//爬虫名称
#define CRAWLER_NAME "miniCrawler"
//网络通信过程中，read函数每次读取的最大数据量
#define MAXLINE 102400
//网络通信的默认超时时间
#define TIMEOUT 5
//对于重定位的最大尝试次数
#define MAXTRYTIMES 3
//Bloom算法里，存储char数组默认大小，实际存储范围MAXBLOOMSIZE*8bit
#define MAXBLOOMSIZE 100000
//单个页面存储文件存储数量的上限阀值
#define MAXFILELIMIT 1000
//URL池中，每次移动URL的最大数量
#define MAX_URLS_ONCE 500
//爬虫单主机爬行深度
#define CRAWLER_DEAPTH 2
//Frontier Back Queue size
#define BACKQUEUE_SIZE 90
//Fronteir Front Queue size
#define FRONTQUEUE_SIZE 20
//允许开启的最大线程数
#define MAX_THREAD_NUMBER 1024
//线程池中线程数量
#define THREAD_NUMBER 20
//存储文件basename，可加路径
#define BASE_NAME "fetchedPage4/file"

//定义一些常量
#define LOCATION "Location"
#define INDEXFILE "indexer"

//种子URL地址
#define SEEDURL "http://www.csdn.net/"
//种子URL优先级 ，必须 < FRONTQUEUE_SIZE
#define SEEDURL_PRIORITY 19

//robots过滤中 Disallow数组的默认大小
#define MAXDISALLOW 100
//主机名过滤参数
#define MAX_HOSTFILTER_NUM 1000
#define HOSTFILTER \
	"www.yahoo.com|weibo.com"
//后缀名过滤参数
#define MAX_SUFFIXFILTER_NUM 100
#define SUFFIXFILTER \
	".doc|.rar|.pdf|.xls|.ppt|.zip|.rm|.avi|.m2p|.wmv|.mp3|.mp4"

//本机机器名字
#define MACHINE_NAME "machineC"
//通信端口
#define PORT 2003
//最大尝试连接指定client次数，若超过此次数，自动废除该client，更新一致性hash节点
#define MAXTRYCONNECT 5

//分布式client配置
#define initClients(vec) \
	vec.push_back(new CNode_s((char*)"machineA",150,(char*)"127.0.0.1"));\
	vec.push_back(new CNode_s((char*)"machineB",80,(char*)"127.0.0.2"));\
	vec.push_back(new CNode_s((char*)"machineC",200,(char*)"127.0.0.3"));\
	vec.push_back(new CNode_s((char*)"machineD",40,(char*)"127.0.0.4"));
//client IP -> port
#define initClientPortMap(portMap) \
	portMap["127.0.0.1"] = 2001;\
	portMap["127.0.0.2"] = 2002;\
	portMap["127.0.0.3"] = 2003;\
	portMap["127.0.0.4"] = 2004;

#endif /* CONFIG_H_ */
