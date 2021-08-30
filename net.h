#ifndef NET_H
#define NET_H

extern struct sockaddr *sock_address(char *host);

extern int socket_net(char *host);
extern int listen_net(int sockfd, int backlog);

extern int accept_net(int sockfd);
extern int connect_net(char *host);

extern int send_net(int sockfd, char *buf, size_t size);
extern int read_net(int sockfd, char *buf, size_t size);

#endif
