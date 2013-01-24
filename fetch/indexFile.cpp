/*
 * indexFile.cpp
 *
 *  Created on: Mar 30, 2012
 *      Author: mayday
 */

#include"indexFile.h"

indexFile::indexFile(const char * file_name , int pflag)
{
	assert(file_name!=NULL);
	strcpy(filename,file_name);
	flag = pflag;
	if (pflag == R)
		fp = fopen(filename,"r");
	else if (pflag == W)
		fp = fopen(filename,"w");
	else if (pflag == A)
		fp = fopen(filename,"a");
	assert(fp!=NULL);
}

void indexFile::set
	(const int num_text , const int size_text ,const char * url_text , const char * time_text)
{
	assert(url_text!=NULL&&time_text!=NULL);
	idx.size = size_text;
	idx.num = num_text;
	strcpy(idx.url,url_text);
	strcpy(idx.timep,time_text);
}

void indexFile::parse
	(const char * text , int & pnum , int & psize , char * purl, char * ptime)
{
	assert(text!=NULL);
	int size_url , size_time;
	int len = strlen(text);
	sscanf(text,"%d%d%d%d", &pnum, &psize, &size_url, &size_time);
	int pos1 = len - size_time - size_url - 1;
	int pos2 = pos1 + size_url;

	if(purl!=NULL)
	{
		strncpy(purl,text+pos1,pos2-pos1);
		purl[pos2-pos1] = '\0';
	}
	if(ptime!=NULL)
	{
		strncpy(ptime,text+pos2+1,len-pos2-1);
		ptime[len-pos2-1] = '\0';
	}
}

bool indexFile::next(indexer & index)
{
	assert(flag == R);
	char *line = NULL;
	size_t len = 0;
	getline(&line,&len,fp);
	if(strlen(line)<=1) return false;
	parse(line,index.num , index.size ,index.url , index.timep);
	return true;
}


void indexFile::writef()
{
	assert(flag != R);
	int size_url = strlen(idx.url);
	int size_time = strlen(idx.timep);
	sprintf(array,"%d %d %d %d %s %s",idx.num,idx.size,size_url,size_time,idx.url,idx.timep);
	fwrite(array,strlen(array),sizeof(char),fp);
	fflush(fp);
}

void indexFile::closef()
{
	fflush(fp);
	fclose(fp);
}


///////////////////////////////////////////////////

scanFile::scanFile(const char * base_name , const char * index_name)
{
	assert(basename!=NULL && index_name!=NULL);
	index = new indexFile(index_name,indexFile::R);

	int len = strlen(base_name);
	basename = new char [len + 1];
	strcpy(basename,base_name);

	filefd = 0;
	setnewfile(0);

	offset.push_back(0);
	page_content = new char [96000];

	init();
}

void scanFile::setnewfile(const int page_num)
{
	close(filefd);
	//重新命名
	char num_str [7];
	sprintf(num_str,"%06d",page_num);
	strcpy(currentfilename,basename);
	strcat(currentfilename,num_str);

	currentfilenumber = page_num;

	//创建 or 打开新文件
	mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	filefd = open(currentfilename,O_RDONLY, mode);
	cout<<filefd<<endl;
}

scanFile::~scanFile()
{
	delete [] page_content;
	delete [] basename;
}
void scanFile::init()
{
	indexer idx;
	int counter = 0;
	int pre = 0;
	//遍历索引文件，记录数据页面偏移量
	while(index->next(idx))
	{
		if(counter%MAXFILELIMIT==0)
		{
			pre = 0;
		}
		offset.push_back(pre+idx.size);
		pagesize.push_back(idx.size);
		pre = offset.back();
		counter++;
	}
}

char * scanFile::locate(const int page_num)
{
	int number = page_num / MAXFILELIMIT;
	if (number!=currentfilenumber)
	{
		//update filefd  , currentfilenumber , currentfilenumber
		setnewfile(number);
	}
	int size_offset = offset.at(page_num-1);
	int size = pagesize.at(page_num-1);

	//找到数据文件的指定序号的开始位置
	lseek(filefd,size_offset,SEEK_SET);
	//从上面的文件位置，进行读取
	read(filefd,page_content,size);
	page_content[size] = '\0';
	return page_content;
}



/*
//Test

int main()
{
	time_t timep;
	time (&timep);
	char * t = asctime(gmtime(&timep));

	char filename [] = "fife10002.idx";
	indexFile index(filename , indexFile::R);
	indexer idx;
	while(index.next(idx))
	{
		cout<<"No.\t"<<idx.num<<"\tSize:"<<idx.size<<endl;
		cout<<"Fetched Url:\t"<<idx.url<<"Fetched Time:\t"<<idx.timep<<endl;
	}
	index.closef();

	scanFile scanner("fetchedPage/taobaofile","fetchedPage/taobaofile.index") ;
	cout<<scanner.locate(232)<<endl;

	return 0;
}
*/
