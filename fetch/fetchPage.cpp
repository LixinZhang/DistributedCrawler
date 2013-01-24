/*
 * fetchPage.cpp
 *
 *  Created on: 2012-3-25
 *      Author: xiaojay
 */

#include"fetchPage.h"

fetchPage::fetchPage(const char * basename)
{
	//初始化属性
	assert(basename!=NULL);
	currentcounter = 0;
	filecounter = 0;
	fetchedcounter = 0;

	int len = strlen(basename);
	filebasename = new char [len+1];
	
	char indexfilename [32];
	strcpy(indexfilename,basename);
	strcat(indexfilename,".index");
	indexfile = new indexFile(indexfilename , indexFile::A);
	
	strcpy(filebasename,basename);
	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	strcpy(currentfilename , filebasename);
	strcat(currentfilename , "000000");
	//打开文件，准备写入
	filefd = open(currentfilename,O_WRONLY | O_CREAT | O_APPEND, mode);
	sockfd = -1;
}

fetchPage::~fetchPage()
{
	close(filefd);
	close(sockfd);
	indexfile->closef();
	delete [] filebasename;
	delete [] indexfile;
}
//建立链接，返回sockfd
int fetchPage::buildConnection(url * u)
{
	assert(u!=NULL);
	int sockfd;
	struct sockaddr_in servaddr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	//cout<<"Sockfd\t"<<sockfd<<endl;
	memset(&servaddr ,0 ,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(80);
	//convert host_name to host_addr, then set in u
	sethostaddr(u);
	if (u->getHostAddress() == NULL) return -1;
	inet_pton( AF_INET ,u->getHostAddress() , &servaddr.sin_addr );

	//set timeout
	struct timeval timeout={TIMEOUT,0};
	int flag = 1;
	int sendopt=setsockopt(sockfd,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
	int recvopt=setsockopt(sockfd,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
	int reuseopt = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int));
	if(sendopt!=0||recvopt!=0||reuseopt<0)
	{
		cout<<"-----something wrong when set timeout.-----"<<endl;
		result[1]++;
		return -1;
	}
	int conn_res =connect(sockfd, (struct sockaddr *)&servaddr , sizeof(servaddr));
	if(conn_res<0)
	{
		cout<<"-----time out or other faults when connect.-----"<<endl;
		result[1]++;
		return -1;
	}
	return sockfd;
}

//一次通信
bool fetchPage::communication(url * u , int trytimes)
{
	if(trytimes>= MAXTRYTIMES )
	{
		cout<<"Try too many times!"<<endl;
		result[0]++;
		return false;
	}
	char recvbuf [MAXLINE];
	//recvbuf[0] = '\0';
	if(sockfd<0) return false;
	//message to be sent
	char msg [1024];
	msg[0] = 0;
	//Send GET protocol to a host with port 80
	sprintf(msg,"GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent:%s\r\n\r\n"
			,u->getFile(),u->getHost(),CRAWLER_NAME);
	int write_res = write(sockfd,msg,strlen(msg));
	if(write_res<0)
	{
		cout<<"-----time out or other faults when write.-----"<<endl;
		result[1]++;
		return false;
	}
	bool dealHead = false;
	//request result from HEAD
	int code = -1;
	//301 or 302 -> Location url
	url * uu = NULL;
	int page_size = 0;
	int n=-1;
	bool another = false;
	while((n=read(sockfd,recvbuf,MAXLINE))>0)
	{
		if(n<0)
		{
			cout<<"-----time out or other faults when read at fetchPage.communication.-----"<<endl;
			result[1]++;
			return false;
		}
		page_size += n;
		recvbuf[n]='\0';
		if(!dealHead)
		{
			code = parser.parseHeader(recvbuf,n);
			if (code == 301 || code == 302)
				uu = parser.parseLocation(recvbuf,n,u);
				//cout<<uu->giveUrl()<<endl;
			if(uu!=NULL)
			{
				close(sockfd);
				sockfd = buildConnection(uu);
				another = communication(uu,trytimes+1);
				// uu was created in parseLocation, so now we must release memory
				//delete uu;
				break;
			}
			dealHead = true;
		}
		if(code==200)
		{
			//write to file
			int write_res = write(filefd,recvbuf,n*sizeof(char));
			if(write_res<=0){
				cout<<"Something is wrong when write to file!"<<endl;
				cout<<"Filefd:\t"<<filefd<<"\t"<<"N:\t"<<n<<endl;
				cout<<"Thread id is "<<pthread_self()<<endl;
			}

			//fetch new local and foreign urls
			parser.parseHref(recvbuf,u,localQueue);
		}
		//recvbuf[0] = '\0';
	}

	if(trytimes==0) 	//只记录第一次
		result[code]++; // record fetched results
	if(code==200)
	{
		currentcounter++;
		fetchedcounter++;
		if(currentcounter>=MAXFILELIMIT)
		{
			currentcounter = 0;
			filecounter++;
			char filecounter_text [20];
			sprintf(filecounter_text,"%06d",filecounter);
			strcpy(currentfilename,filebasename);
			strcat(currentfilename,filecounter_text);
			close(filefd);
			filefd = open(currentfilename,O_WRONLY | O_CREAT | O_APPEND, mode);
		}
		//record the fetched time
		time_t timep;
		time (&timep);
		char * t = asctime(gmtime(&timep));
		char * url_text = u->giveUrl();
		indexfile->set(fetchedcounter,page_size,url_text,t);
		indexfile->writef(); // record
		//控制台结果打印
		//cout<<"Fetched\t"<<url_text<<"\tDeapth\t"<<u->getDepth()<<"\tPriority\t"<<u->getPriority()
		//		<<"\tWork Thread is "<<pthread_self()<<endl;
		return true;
	}
	else return another;
	return true;
}

// set host address to url obj
// 设置IP地址
void fetchPage::sethostaddr(url * u)
{
	string key = string(u->getHost());
	//DNS cache
	pthread_mutex_lock(&dnsCache_mutex);
	int haskey = dnsCache->count(key);
	pthread_mutex_unlock(&dnsCache_mutex);
	if(haskey>0)
	{
		pthread_mutex_lock(&dnsCache_mutex);
		u->setHostAddress((*dnsCache)[key].c_str());
		pthread_mutex_unlock(&dnsCache_mutex);
	}
	else
	{
		char  serverAddress [20];
		struct hostent * ip_addr = gethostbyname(u->getHost());
		if(ip_addr == NULL){
			return ;
		}
		inet_ntop(ip_addr->h_addrtype, ip_addr->h_addr, serverAddress, sizeof(serverAddress));
		string value = string(serverAddress);
		pthread_mutex_lock(&dnsCache_mutex);
		(*dnsCache)[key] = value;
		pthread_mutex_unlock(&dnsCache_mutex);
		u->setHostAddress(serverAddress);
	}
}

//fetch robots.txt文件，并进行解析 , 返回robots过滤器
robotsFilter * fetchPage::fetchRobots(url * u)
{
	close(sockfd);
	sockfd = buildConnection(u);
	if (sockfd<=0) return NULL;
	char recvbuf[10240];
	char whole[20480];
	char msg [1024];
	whole[0] = '\0';//So important!!!!!! 后面用的strcat
	//Send HTTP GET protocol to a host with port 80
	sprintf(msg,"GET %s HTTP/1.0\r\nHost: %s\r\nUser-Agent:%s\r\n\r\n"
			,"/robots.txt",u->getHost(),CRAWLER_NAME);

	int write_res = write(sockfd,msg,strlen(msg));
	if(write_res<0)
	{
		cout<<"-----time out or other faults when write.-----"<<endl;
		result[1]++;
		return NULL;
	}
	int n;
	while((n=read(sockfd,recvbuf,10240))>0)
	{
		if(n<0)
		{
			cout<<"-----time out or other faults when read.-----"<<endl;
			return NULL;
		}
		recvbuf[n]='\0';
		strcat(whole,recvbuf);
	}
	int code = parser.parseHeader(whole,100);

	if (code!=200) return NULL;
	int pos = 0;
	int len = strlen(whole);
	// find "\r\n\r\n"
	for (pos=0;pos<len-3;pos++)
	{
		if(whole[pos]=='\r'&&whole[pos+1]=='\n'
				&&whole[pos+2]=='\r'&&whole[pos+3]=='\n')
			break;
	}
	pos+=4;
	robotsFilter * robots = new robotsFilter(u->getHost(),whole+pos);
	return robots;
}

int fetchPage::statisticsCount(const int code)
{
	return result.count(code)>0?result[code]:0;
}

//打印统计结果
void fetchPage::displayStatistics()
{
	map<int,int>::iterator iter = result.begin();
	while(iter!=result.end())
	{
		cout<<iter->first<<"\t:"<<iter->second <<endl;
		iter++;
	}
}

//打印DNS缓存记录
void fetchPage::displayDnsCache()
{
	map<string,string>::iterator iter = dnsCache->begin();
	while(iter!=dnsCache->end())
	{
		cout<<iter->first<<"\t:"<<iter->second<<endl;
		iter++;
	}
}

//抓取u指向的页面，爬行深度由u决定，采用广度优先的爬取策略
void fetchPage::fetch(url * u)
{
	long hasfetched = 0;
	url * feedurl = new url(u->getHost() , u->getPort() , u->getFile() , u->getDepth() , u->getPriority());
	localQueue.push(feedurl);
	url * uu;
	string key = string(u->getHost());
	bool useRobot = false;
	/*
	robotsFilter * filter = NULL;
	//DNS cache ,robots.txt 只解析一次
	pthread_mutex_lock(&robotsCache_mutex);
	int haskey = robotsCache->count(key);
	pthread_mutex_unlock(&robotsCache_mutex);
	if(haskey>0)
	{
		pthread_mutex_lock(&robotsCache_mutex);
		filter = robotsCache->at(key);
		pthread_mutex_unlock(&robotsCache_mutex);
	}
	else
	{
		filter = fetchRobots(u);
		if (filter!=NULL)
		{
			pthread_mutex_lock(&robotsCache_mutex);
			(*robotsCache)[key] = filter;
			pthread_mutex_unlock(&robotsCache_mutex);
		}
	}
	if(filter!=NULL)
	{
		//添加到HtmlParser的过滤器列表中
		parser.filterlist.push_back(filter);
		useRobot = true;
	}


	*/
	int maxurls_once = 200;
    while(!localQueue.empty())
    {
    	maxurls_once--;
    	if(maxurls_once<0)
    	{
    		while(!localQueue.empty())
    		{
    			uu = localQueue.front();
    			localQueue.pop();
    			delete uu;
    		}
    		break;
    	}
    	uu = localQueue.front();
    	//cout<<"Now is fetching\t"<<uu->giveUrl()<<endl;
    	//因为http协议默认不会保持连接，也就是说，每次请求必须重新建立连接
    	sockfd = buildConnection(uu);
    	if(sockfd <= 0)
    	{
			localQueue.pop();
			delete uu;
			cout<<"##############ERROR##############"<<endl;
			continue;
    	}
    	bool res = communication(uu ,0);
    	hasfetched++;
    	localQueue.pop();
    	delete uu;
    	//打印中间结果
    	/*
    	if(res)
    		cout<<"已经爬取\t"<<hasfetched<<"\t剩余\t"<<localQueue.size()<<endl;
    	*/
    	close(sockfd);
    }
	if(useRobot){
		//parser.filterlist.erase(parser.filterlist.end());
		useRobot = false;
	}
	close(sockfd);
}

#include <time.h>
#include <sys/time.h>
/*
int testfetchPage()
{
	char * seedurl = SEEDURL;
	fetchPage fetch("fetchedPage/csefile");
	url * u = new url(seedurl,CRAWLER_DEAPTH,SEEDURL_PRIORITY);

	pthread_mutex_lock(&frontier_mutex);
	frontier->pushurl(u);
	pthread_mutex_unlock(&frontier_mutex);

	while(true)
	{
		pthread_mutex_lock(&frontier_mutex);
		u = frontier->popurl();
		pthread_mutex_unlock(&frontier_mutex);

		if(u==NULL) break;
		timeval now ;
		while(true)
		{
			gettimeofday(&now,NULL);
			cout<<now.tv_sec<<"\t"<<now.tv_usec<<endl;
			cout<<u->getTe().tv_sec<<"\t"<<u->getTe().tv_usec<<endl;
			if(timercmp(&now,&u->getTe(),>)){
				cout<<"Now seed url is "<<u->giveUrl()<<"\t priority is "<<u->getPriority()<<endl;
				fetch.fetch(u); break;
			}
			else {
				sleep(1);
			}
		}
	}
	return 0;
}

*/
