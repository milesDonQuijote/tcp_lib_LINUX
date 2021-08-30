#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "net.h"


#define PORT_LEN 4
#define IPV4_LEN 14
#define ERROR -1

int _parse_address(char *host, char *ipv4, char *port);

struct sockaddr *sock_address(char *host)
{
	//initialization of the address structure 
	//for transfer in functions as socket and connect

	char ipv4[16];
	char port[6];
	static struct sockaddr_in addr;

	//seperation of ip address and port
	_parse_address(host, ipv4, port);

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(port));
	addr.sin_addr.s_addr = inet_addr(ipv4);

	return (struct sockaddr *) &addr;
}

int socket_net(char *host)
{
	//socket creation and binding

	int sockfd;
	struct sockaddr *addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	addr = sock_address(host);
	socklen_t addrlen = sizeof *addr;

	return bind(sockfd, addr, addrlen) == ERROR?
		ERROR: sockfd;
}

int listen_net(int sockfd, int backlog)
{
	// listening port

	 return listen(sockfd, backlog);
}

int accept_net(int sockfd)
{
	//triple handshake
	return accept(sockfd, NULL, NULL);
}

int connect_net(char *host)
{
	//connect to server
	int sockfd;
	socklen_t addrlen;
	struct sockaddr *addr;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == ERROR)
		return ERROR;

	addr = sock_address(host);
	addrlen = sizeof *addr;

	return connect(sockfd, addr, addrlen) == ERROR?
		ERROR: sockfd;
}

int send_net(int sockfd, char *buf, size_t size)
{
	//sending data between server and client

	int sended; //number of bytes successfully sent
	sended = send(sockfd, buf, size, 0);

	if(sended == ERROR)
	{
		perror("SENDING ERROR:");
		exit(EXIT_FAILURE);
	}

	return sended;
}

int read_net(int sockfd, char *buf, size_t size)
{
	//readin data between server and client

	int readed; //number of bytes successfully read
	readed = recv(sockfd, buf, size, 0);

	if(readed == ERROR)
	{
		perror("READING ERROR:");
		exit(EXIT_FAILURE);
	}

	return readed;
}

int _parse_address(char *host, char *ipv4, char *port)
{
	//seperation of ip address(ipv4) and port
	//host=0.0.0.0:443|ipv4=0.0.0.0|port=443

	int i, j;
	i = j = 0;

	for(; host[i] != ':'; i++)
	{
		if(host[i] == '\0' || i > IPV4_LEN)
			return ERROR;
		ipv4[i] = host[i];
	}
	ipv4[i++] = '\0';

	for(; host[i] != '\0';)
	{
		if(host[i] == '\0' || j > PORT_LEN)
			return ERROR;
		port[j++] = host[i++];
	}

	return 0;
}
