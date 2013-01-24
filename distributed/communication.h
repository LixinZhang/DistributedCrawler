/*
 * communication.h
 *
 *  Created on: Apr 30, 2012
 *      Author: mayday
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <queue>
#include <map>
#include <string>
#include "cJSON.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <time.h>

#include "../config.h"
#include "../urlFrontier/url.h"
#include "../urlFrontier/urlfrontier.h"
#include "../filter/bloom.h"
#include "../fetch/htmlParser.h"

extern pthread_mutex_t frontier_mutex;
extern pthread_mutex_t bloom_mutex;
extern urlfrontier * frontier;
extern Bloom * bloom;
extern map<string,int> route_error_result;
extern CConHash * conhash;
extern map<string , CNode_s *> CNodes_sMap;

class communication
{
public:
	void send(string client_address , int port , queue<url *> &);
	void recv();
	char * encoding (queue<url *> &);
	queue<url *> decoding(const char * decoding_text);
};


#endif /* COMMUNICATION_H_ */
