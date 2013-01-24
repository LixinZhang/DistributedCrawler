/*
 * robotFilter.cpp
 *
 *  Created on: Mar 31, 2012
 *      Author: mayday
 */

#include "urlFilter.h"

// 初始化相关参数
robotsFilter::robotsFilter(const char * host_name , const char * robots_txt)
{
	assert(host_name!=NULL && robots_txt!=NULL);
	int len = strlen(host_name);
	hostname = new char [len + 1];
	strcpy(hostname,host_name);

	len = strlen(robots_txt);
	robotstxt = new char [len + 1];
	strcpy(robotstxt,robots_txt);

	disallow_size = 0;
	allow_size = 0;
	crawldelay = 0;

	parseRobots();
}

robotsFilter::~robotsFilter()
{
	delete [] hostname;
	delete [] robotstxt;
	for(int i=0;i<disallow_size;i++)
	{
		delete [] disallow[i];
	}
	for(int i=0;i<allow_size;i++)
	{
		delete [] allow[i];
	}
}

//
char* robotsFilter::strlwr( char* str )
{
	char* orig = str;
	// process the string
	for (;*str != '\0'; str++ )
		*str = tolower(*str);
	return orig;
}


//解析robots文本
void robotsFilter::parseRobots()
{
	//cout<<robotstxt<<endl;
	//key:value 
	char key [32];
	char value [100];
	int i,j;
	int posl = 0, posm = 0 ,posr =0;
	int len = strlen(robotstxt);
	bool hasAgent = false;
	//总体思路，确定三个标志的位置，然后分割字符串. ——>posl( key )posm( value )posr
	while(posl<len && posm<len && posr<len)
	{
		//找到第一个不为空格和换行符的字符位置，确定posl
		while(posl<len && (robotstxt[posl]==' '
				|| robotstxt[posl]=='\n' || robotstxt[posl]=='\r')) posl++;
		//以#开头的，直接过滤掉该行
		if(robotstxt[posl]=='#')
		{
			while(posl<len && robotstxt[posl]!='\n') posl++;
			continue;
		}
		//找‘：’,确定posm
		posm = posl+1;
		while(posm<len && robotstxt[posm]!=':') posm++;
		//找换行符位置，确定posr
		posr = posm+1;
		while(posr<len && robotstxt[posr]!='\n') posr++;

		for(j=0,i=posl;i<posm;i++)
		{
			if(robotstxt[i]!=' '&&robotstxt[i]!='\t'&&robotstxt[i]!='\r'&&robotstxt[i]!='\n')
				key[j++] = robotstxt[i];
		}
		key[j] = '\0';
		for(j=0,i=posm+1;i<posr;i++)
		{
			if(robotstxt[i]!=' '&&robotstxt[i]!='\t'&&robotstxt[i]!='\r'&&robotstxt[i]!='\n')
				value[j++] = robotstxt[i];
		}
		value[j]='\0';
		posl = posr;
		//cout<<key<<"\t"<<value<<endl;
		//确定user-agent，是否针对本爬虫
		if(strcmp(strlwr(key),"user-agent")==0){
			if(strcmp(value,"*")==0||strcmp(value,CRAWLER_NAME)==0)
			{
				hasAgent = true;
			}
			else hasAgent = false;
		}
		if(hasAgent)
		{
			int len_val = strlen(value);
			if(len_val<=0) continue;
			if(strcmp(strlwr(key),"disallow")==0 && disallow_size<MAXDISALLOW)
			{
				disallow[disallow_size] = new char [len_val+1];
				strcpy(disallow[disallow_size],strlwr(value));
				disallow_size++;
			}
			else if (strcmp(strlwr(key),"allow")==0 && allow_size<MAXDISALLOW)
			{
				allow[allow_size] = new char [len_val+1];
				strcpy(allow[allow_size],strlwr(value));
				allow_size++;
			}
			else if(strcmp(strlwr(key),"craw-delay")==0)
			{
				crawldelay = 0;
				int len_val = strlen(value);
				for(int i=0;i<len_val;i++)
				{
					crawldelay = crawldelay * 10 + value[i]-'0';
				}
			}
		}
	}
}

void robotsFilter::printDisallow()
{
	for(int i=0;i<disallow_size;i++)
	{
		cout<<disallow[i]<<endl;
	}
}

bool robotsFilter::robotsMatch(char * match ,char * file)
{
	int len_match = strlen(match);
	if(len_match<=1) return true;
	int len_file =  strlen(file);
	if(len_match>len_file) return false;
	// ends with '$'
	if(match[len_match-1]=='$')
	{
		int pos_match = 0;
		char head [50],tail[20];
		pos_match = len_match - 1;
		while(pos_match>=0&&match[pos_match]!='*') pos_match --;
		strncpy(head,match,pos_match);
		head[pos_match] = '\0';
		strncpy(tail,match+pos_match+1,len_match-pos_match-2);
		tail[len_match-pos_match-2] = '\0';
		int lhead = pos_match;
		int ltail =	len_match-pos_match-2;
		int i = 0;
		while(i<lhead && head[i]==file[i]) i++;
		if(i!=lhead) return false;
		i=0;
		while(i<ltail && tail[i]==file[len_file-ltail+i]) i++;
		if(i!=ltail) return false;
		return true;
	}

	// 统计 '*'
	int starlist [20] , starcount=0;
	for(int i=0;i<len_match;i++)
		if(match[i]=='*')
			starlist[starcount++] = i;
	//if have no star
	if(starcount==0)
	{
		int i=0;
		while(i<len_match && match[i]==file[i]) i++;
		if(i!=len_match) return false;
		else return true;
	}
	// if have star
	/*
	算法如下，首先按照'*',分割为多给子串
	然后各个子串需按序匹配到file文本，且每个子串按首次匹配为准
	*/	
	int i=0;
	//test head
	while(i<starlist[0]&&match[i]==file[i]) i++;
	if (i!=starlist[0]) return false;
	//test tail
	if(match[len_match-1]!='*')
	{
		int length = len_match - (starlist[starcount-1] + 1);
		i = 0;
		while(i<length && match[len_match - i -1] == file[len_file - i -1]) i++;
		if (i!=length) return false;
	}

	// match each star
	int start_match , end_match , start_file  = starlist[0];
	char case_str [50];
	for(int i=1;i<starcount;i++)
	{
		start_match = starlist[i-1]+1;
		end_match = starlist[i];
		strncpy(case_str,match+start_match,end_match-start_match);
		case_str[end_match-start_match] = '\0';
		int i = start_file;
		int j = 0;
		//match case_str , perhaps KMP algorithms is better than mine
		while(i<len_file && j<(end_match-start_match))
		{
			if(file[i]==case_str[j]){
				++i;++j;
			}else{
				i = i-j+1;
				j = 0;
			}
		}
		if(j==(end_match-start_match))
		{
			start_file = i;
		}else return false;
	}
	return true;
}

// test if u->file matches robots' disallow ,return false ; otherwise true
bool robotsFilter::filter(url * u)
{
	/*
	 *robots.txt里面可能有Allow，也可能有Disallow
	 *其实如果两个都设置的话，会有一点歧义，crawler不知道到底应不应该爬
	 *这里给出的方法是：两个都进行测试，只要通过一个就ok。
	 */
	if(disallow_size<=0 && allow_size<=0 ) return true;
	if(u->getFile()==NULL) return true;
	bool pass_disallow = true;
	if(disallow_size>0)
	{
		if(strcmp(disallow[0],"/")==0 || strcmp(disallow[0],"*")==0
				||strcmp(disallow[0]," ")==0 ||strcmp(disallow[0],"")==0)
			pass_disallow = false;
		else
		{
			for(int i=0;i<disallow_size;i++)
			{
				if (robotsMatch(disallow[i],u->getFile())) {
					pass_disallow = false;
					break;
				}
			}
		}
	}
	else pass_disallow = false;
	if(pass_disallow==true) return true;

	bool pass_allow = false;
	if (allow_size>0)
	{
		if(strcmp(allow[0],"/")==0 || strcmp(allow[0],"*")==0) pass_allow = true;
		else
		{
			for(int i=0;i<allow_size;i++)
			{
				if (robotsMatch(allow[i],u->getFile()))
				{
					pass_allow = true;
					break;
				}
			}
		}
	}
	return pass_allow;
}

bool robotsFilter::filter(const char * url_text)
{
	char tmp [150];
	strcpy(tmp,url_text);
	url * u = new url(tmp,0,0);
	bool res = filter(u);
	delete u;
	return res;
}

