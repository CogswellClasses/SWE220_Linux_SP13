/*
  listen: listen to chat room
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>

#include "chatPorts.h"

void main()
{
  int sockfd;
  struct sockaddr_in self;
  char buffer[MAXBUF];

  /*---Create datagram socket---*/
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("Socket");
    exit(errno);
  }

  /*---Initialize address/port structure---*/
  bzero(&self, sizeof(self));
  self.sin_family = AF_INET;
  self.sin_port = htons(LISTENPORT);
  self.sin_addr.s_addr = INADDR_ANY;
  
  int optval = 1;
  setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  /*---Assign a port number to the socket---*/
  printf ("bind incoming socket to %s:%d\n",  inet_ntoa(self.sin_addr), ntohs(self.sin_port));

  if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 ) {
    perror("socket--bind");
    exit(errno);
  }

  /*---Forever... ---*/
  while (1) {
    struct sockaddr_in client_addr;
    int addrlen=sizeof(client_addr);
		
    recvfrom (sockfd, buffer, sizeof(buffer), 0,
	      (struct sockaddr*) &client_addr, &addrlen);
    printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
    printf ("received: %s", buffer);
  }

  /*---Clean up (should never get here!)---*/
  close(sockfd);
}

