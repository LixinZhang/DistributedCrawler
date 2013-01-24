/*
 * indexFile.h
 *
 *  Created on: Mar 30, 2012
 *      Author: mayday
 */

#ifndef INDEXFILE_H_
#define INDEXFILE_H_

#include<sys/stat.h>
#include<fcntl.h>
#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<time.h>
#include<stdlib.h>
#include<vector>

#include"../config.h"

//索引项
struct indexer
{
	int num; //存储号码
	char url [1024]; //url文本
	char timep [36]; //抓取时间文本
	int size; //存储页面的大小
};

/*
 * 索引文件工具类
 */
class indexFile
{
public:
	//构造函数，file_name指定索引文件名， pflag指定文件读写属性
	indexFile(const char * file_name , int pflag);
	//设置索引参数
	void set(const int size_text , const int num_text ,const char * url_text , const char * time_text);
	//根据指定格式，对索引文件记录text进行解析
	void parse (const char * text , int & pnum , int & psize , char * purl, char * ptime);
	//迭代遍历利索引项
	bool next (indexer & index);
	//写文件
	void writef();
	//读文件，按行读
	void readnextline();
	//关闭文件
	void closef();
	//读写属性
	const static int R = 0;
	const static int W = 1;
	const static int A = 2;
private:
	int flag;
	char filename [30];
	char array [1024 + 20 + 36];
	indexer idx;
	mode_t mode;
	FILE * fp;
};

/*
 * 根据索引文件遍历对应页面储存文件
 */
class scanFile
{
public:
	//构造函数，初始化相关参数 ，basename指定页面存储文件名字 ，index_name指定索引文件名字
	scanFile(const char * base_name , const char * index_name);
	~scanFile();
	//根据索引文件中的页面记录号，定位页面，返回页面数据
	char * locate(const int page_num);
private:
	//初始化offset，【agesize
	void init();
	//重新定位文件
	void setnewfile(const int file_num);
	indexFile * index;
	int filefd;
	char * basename;
	char currentfilename [32];
	int currentfilenumber;
	vector<int> offset;
	vector<int> pagesize;
	char * page_content;
};

#endif /* INDEXFILE_H_ */
