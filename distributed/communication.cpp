/*
 * communication.cpp
 *
 *  Created on: Apr 30, 2012
 *      Author: mayday
 */
#include "communication.h"

//向指定client 派送 url 队列序列化文本
void communication::send(string client_address , int port , queue<url *> & urlQueue)
{
	if(urlQueue.size()<=0) return ;
	char * send_text = encoding(urlQueue);
    int sockfd;
    struct sockaddr_in servaddr;
    if( ( sockfd = socket( AF_INET , SOCK_STREAM , 0 ) ) < 0 ) {
        printf( "socket error\n" );
    }
    memset( &servaddr , 0 , sizeof( servaddr ) );
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons( port );//指定Server端的端口号

    char serverAddress [30];
    strcpy(serverAddress,client_address.c_str());
    //判断指定的ip地址是否有错误
    if( inet_pton( AF_INET ,serverAddress , &servaddr.sin_addr ) <= 0 )  {
        printf( "inet_pton error for %s\n" , serverAddress );
    }
    if( connect( sockfd , (struct sockaddr *)&servaddr , sizeof( servaddr ) ) < 0 )  {
    	route_error_result[client_address] ++;
        cout<<"连接 "<<client_address<<"失败,"<<"已经失败次数:"<<route_error_result[client_address]<<endl;
        //超过最大尝试连接次数，自动废除该client，更新一致性hash节点环
        if(route_error_result[client_address]>MAXTRYCONNECT)
        {
        	conhash->delNode_s(CNodes_sMap[client_address]);
        	cout<<"已经删除\t"<<client_address<<"\t节点"<<endl;
        }
        return ;
    }
    int write_res = write(sockfd,send_text ,strlen(send_text) * sizeof(char));
    if(write_res<0) {
    	cout<<"Something is wrong while sending to other clients."<<endl;
    }
    delete [] send_text;
    close(sockfd);
}


//接收其他client传过来的 URL list
void communication::recv()
{
    char buffer[MAXLINE];
    char whole_text [MAXLINE*5];
    int listenfd=socket(AF_INET,SOCK_STREAM,0);//定义socket，指向ipv4地址的字节流套接口

    struct sockaddr_in serverAddr;
    memset(&serverAddr,0,sizeof(serverAddr));//sockAddr_in 进行初始化
    serverAddr.sin_family=AF_INET;
    serverAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    serverAddr.sin_port=htons(PORT);
    if(bind(listenfd,(struct sockaddr *) &serverAddr,sizeof(serverAddr))==-1)
    {
        printf("There is an error during binding\n");
        return ;
    }
    else
    {
        printf("Bind successfully!!!\n");
    }
    //对listenfd进行监听，从最初建立时的主动套接口（用于进行connect的套接口）转化为被动套接口（接受连接）
    listen(listenfd,100);
    int connectfd;
    socklen_t addrlen;
    struct sockaddr_in connectAddr;
    memset(&connectAddr,0,sizeof(connectAddr));
    printf("Be ready to accept a connection!\n");

    // 实例化一个url checker
    htmlParser * checker = new htmlParser();

    while(1)
    {
        connectfd=accept(listenfd,(struct sockaddr * )&connectAddr,&addrlen);//接受client端一个请求的socket
        char addr_buffer [100];
        //const char * clientAddress=inet_ntop(AF_INET,&connectAddr.sin_addr,addr_buffer,sizeof(addr_buffer));//获取客户端的ip地址
        int n;
        //接收其他client传过来的json文本，并存入whole_text
        whole_text [0] = '\0'; // that is important
        while((n=read(connectfd,buffer,MAXLINE))>0)
        {
        	buffer[n] = '\0';
        	strcat(whole_text , buffer);
        }
        //解析whole_text,convert to url Queue
        queue<url *> urlQueue = decoding(whole_text);
        cout<<"Received\t"<<urlQueue.size()<<" urls\t"<<"From\t"<<connectAddr.sin_port<<endl;
        //Add to url Frontier
        if (urlQueue.size()>0)
        {
        	url * newurl = NULL;
            while(!urlQueue.empty())
            {
            	newurl = urlQueue.front();
            	//check url
            	if(newurl!=NULL&&newurl->isValid()
            			&&!checker->urlseen(newurl)&&checker->urlfilter(newurl))
            	{
            		//Add to url frontier
					pthread_mutex_lock(&frontier_mutex);
					frontier->pushurl(newurl);
					pthread_mutex_unlock(&frontier_mutex);
            	}
            	urlQueue.pop();
            }
        	pthread_mutex_unlock(&frontier_mutex);
        }
        close(connectfd);
    }
    close(listenfd);
}

// 将URL队列 序列化为 json 文本
char * communication::encoding (queue<url *> & urlQueue)
{
	cJSON * root ;
	root = cJSON_CreateArray();
	while(!urlQueue.empty())
	{
		url * u = urlQueue.front();
		cJSON * item = cJSON_CreateObject();
		cJSON_AddStringToObject(item,"HOST",u->getHost());
		cJSON_AddStringToObject(item,"FILE",u->getFile());
		cJSON_AddNumberToObject(item,"PORT",u->getPort());
		cJSON_AddNumberToObject(item,"PRIORITY",u->getPriority());
		cJSON_AddNumberToObject(item,"DEPTH",u->getDepth());
		cJSON_AddItemToArray(root ,item);
		urlQueue.pop();
	}
	char * out = cJSON_PrintUnformatted(root);
	cJSON_Delete(root);
	return out;
}

// 将json文本反序列化为 url 队列
queue<url *> communication::decoding(const char * decoding_text)
{
	assert(decoding_text!=NULL);
	cJSON * json_arr;
	cJSON * json_item;
	cJSON * host , * file ,*  port ,*  priority ,* depth;

	queue<url *> urlqueue;

	json_arr=cJSON_Parse(decoding_text);
	int size = cJSON_GetArraySize(json_arr);
	for( int i=0;i<size;i++)
	{
		json_item = cJSON_GetArrayItem(json_arr,i);
		if (!json_item) {
			printf("Error before: [%s]\n",cJSON_GetErrorPtr());
		}
		else
		{
			//解析json item ，拼装成url， Add to urlQueue
			host = cJSON_GetObjectItem(json_item,"HOST");
			file = cJSON_GetObjectItem(json_item,"FILE");
			port = cJSON_GetObjectItem(json_item,"PORT");
			priority = cJSON_GetObjectItem(json_item,"PRIORITY");
			depth = cJSON_GetObjectItem(json_item,"DEPTH");
			url * u = new url(host->valuestring , port->valueint ,file->valuestring , depth->valueint ,priority->valueint);
			urlqueue.push(u);
		}
	}
	cJSON_Delete(json_arr);
	return urlqueue;
}
