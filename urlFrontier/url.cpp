#include "url.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>

static const int hashSize = 10000;

/*
 * Some methods for help
 */

int strToint(char * str , int start , int end )
{
	int num = 0;
	for(int i=start;i<=end;i++)
	{
		if(str[i]<'0' || str[i]>'9') return 0;
		num = num*10 + str[i]-'0';
	}
	return num;
}

/*
 * To lower
 */
void toLower(char * u)
{
	int len = strlen(u);
	for(int i=0;i<len;i++)
	{
		u[i] = tolower(u[i]);
	}
}

/*
 * Constructor: Parses an url
 */

url::url(char * host , int port , char * file)
{
	orgurl = NULL;
	rootPath = NULL;
	int len = strlen(host);
	this->host = new char [len+1];
	strcpy(this->host , host );
	//len = strlen(port);
	this->port = port;
	len = strlen(file);
	this->file = new char[len+1];
	strcpy(this->file , file);

	this->hostaddress = NULL;
}

url::url(char * host , int port , char * file , int depth , int priority)
{
	assert(file!=NULL&&host!=NULL);
	orgurl = NULL;
	rootPath = NULL;
	int len = strlen(host);
	this->host = new char [len+1];
	strcpy(this->host , host );
	//len = strlen(port);
	this->port = port;
	len = strlen(file);
	this->file = new char[len+1];
	strcpy(this->file , file);

	this->hostaddress = NULL;
	this->depth = depth;
	this->priority = priority;
}


url::url(char *u , int depth , int priority)
{
	orgurl = NULL;
	rootPath = NULL;
	int len = strlen(u);
	orgurl = new char [len+1];
	strcpy(orgurl,u);
	toLower(orgurl);
	this->port = 80;
	this->host = NULL;
	this->file = NULL;
	this->depth = depth;
	this->priority = priority;
	if(strncmp(orgurl,"https://",8)==0)
	{
		this->parse(orgurl+8);
	}
	else if (strncmp(orgurl,"http://",7)==0)
	{
		this->parse(orgurl+7);
	}
	else
	{
		this->parse(orgurl);
	}
	this->hostaddress = NULL;
}

url::~url()
{
	if(host!=NULL) delete [] host;
	if(file!=NULL) delete [] file;
	if(orgurl!=NULL) delete [] orgurl;
	if(rootPath!=NULL) delete [] rootPath;
}



/*
 * Parse a url like 'www.baidu.com:80/s?wd=mayday'
 */
void url::parse(char * s)
{
	int pos = 0 , pos2 = 0;
	int len = strlen(s);
	while(s[pos]!=':'&&s[pos]!='/'&&s[pos]!='\0') pos++;
	this->host = new char [pos+1];
	strncpy(this->host,s,pos);
	this->host[pos] = '\0';
	if(s[pos]==':')
	{
		pos2 = pos;
		while(s[pos2]!='/'&&s[pos2]!='\0') pos2++;
		this->port = strToint(s , pos+1 , pos2-1);
	}
	else pos2 = pos ;
	if(pos2>0&&s[pos2]!='\0')
	{
		this->file = new char[len-pos2+1];
		strcpy(this->file ,s+pos2);
	}
	else
	{
		//this->file = new char[1];
		//strcpy(this->file,"");
		this->file = new char [2];
		strcpy(this->file,"/");
	}
}

/*
 *  return a hashcode for this url
 */

int url::hashCode()
{
	int h = port;
	int i = 0;
	while(host[i]!='\0')
	{
		h = 31*h + host[i];
		i++;
	}
	i = 0;
	while(file[i]!='\0')
	{
		h = 31*h + file[i];
		i++;
	}
	return h % hashSize;
}


void url::print()
{
	if(file!=NULL)
	{
		printf("http://%s:%d%s",host,port,file);
	}
	else
	{
		printf("http://%s:%d",host,port);
	}
	
}

/*
 *  return a char * representation of the url
 *	Give means that you have to delete the string yourself
 */
char * url::giveUrl()
{
	if(!isValid()) return NULL;
	if(orgurl!=NULL) return orgurl;
	int len;
	char * buf = orgurl;
	if(file==NULL)
	{
		len = strlen(host)+7+1+9;
		buf = new char [len];
		sprintf(buf,"http://%s:%d",host,port);
	}
	else
	{
		len = strlen(host)+strlen(file) + 7 + 1 + 9;
		buf = new char [len];
		sprintf(buf,"http://%s:%d%s",host,port,file);
	}
	return buf;
}

/*
 * The url is a valid or not
 */
bool url::isValid()
{
	if(host == NULL || strcmp(host,"")==0) return false;
	if(strlen(host)<=3) return false;
	if(strcmp(host,"(null)")==0) return false;
	if(strchr(host,'.')==NULL) return false;
	if(file==NULL) return false;
	if(strcmp(file,"#")==0) return false;
	if(strstr(file,"javascript")!=NULL) return false;
	return true;
}

int url::writeUrl(char * buf)
{
	if(port != 80)
	{
		return sprintf(buf,"http://%s:%d%s\n",host,port,file);
	}
	else
	{
		return sprintf(buf,"http://%s%s\n",host,file);
	}
}


void url::setHostAddress(const char * addr)
{
	if (hostaddress!=NULL)
	{
		delete [] hostaddress;
	}
	int len  = strlen(addr);
	hostaddress = new char[len+1];
	strcpy(hostaddress,addr);
}


char * url::getRootPath()
{
	if(rootPath != NULL) return rootPath;
	char * url_text = giveUrl();
	if(url_text==NULL) return NULL;
	int len = strlen(url_text);
	int pos = 0;
	for(pos=len-1;pos>=0;pos--)
	{
		if (url_text[pos]=='/') break;
	}
	if (pos<0)
	{
		return this->getHost();
	}
	rootPath = new char [pos+1];
	strncpy(rootPath,url_text,pos);
	rootPath[pos] = '\0';
	return rootPath;
}



//Test Url

#include<iostream>
using namespace std;
int main6()
{
	url * u = new url("http://www.seu.edu.cn",23,3);
//	url * u = new url("http://aaaa:80",23,3);
//	url * u = new url("http://aaaa/",23,3);
//	url * u = new url("http://aaaa/bbb",23,3);
	cout<<u->giveUrl()<<endl;
	//if(u->getFile()!=NULL)
	cout<<u->getHost()<<endl;
	cout<<u->getFile()<<endl;
	u->print();
	delete u;
	return 0;
}
