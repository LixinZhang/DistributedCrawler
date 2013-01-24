/*
 * url.h
 *
 *  Created on: 2012-2-24
 *      Author: xiaojay
 */

#ifndef URL_H_
#define URL_H_

#include <sys/types.h>
#include <sys/time.h>
class url
{
private:
	//name of the host
	char * host;
	//filename
	char * file;
	//original url string
	char * orgurl;
	// convert hostname to a host address
	char * hostaddress;
	//port number ,defaut 80
	int port;
	//depth of the url to be crawled
	int depth;
	//means the importance ,and a url's priority is higher and it will be fetched earlier
	int priority;
	// the earliest time of the url to be fetched
	timeval te;
	//parse the url to a format like "http://host:port/file"
	void parse(char *s);
	char * rootPath;
public:
	//Constructor
	url(char *u , int depth , int priority);
	url(char * host , int port , char * file);
	url(char * host , int port , char * file , int depth , int priority);
	url(char * line);
	~url();
	/*Is is a valid url ?*/
	bool isValid();
	void print();

	//some inline function
	inline char *getHost() {return this->host ;}
	inline int getPort() {return this->port ;}
	inline char *getFile() {return this->file ;}
	inline int getDepth() {return this->depth ;}
	inline int getPriority() {return this->priority;}
	inline timeval getTe() {return this->te;}
	inline void setTe(timeval te) {this->te = te;}
	// Give a representation of the url
	char * giveUrl();
	//Write a url in a buffer , returns the size of what has been written (not including '\0')
	int writeUrl(char * buf);
	int hostHashCode();
	int hashCode();
	char * getRootPath();

	inline char *getHostAddress() {return this->hostaddress;}
	void setHostAddress(const char * addr);
};

#endif /* URL_H_ */
