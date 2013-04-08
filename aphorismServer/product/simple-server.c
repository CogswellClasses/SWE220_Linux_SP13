/*****************************************************************************/
/*** simple-server.c                                                       ***/
/***                                                                       ***/
/*****************************************************************************/

/**************************************************************************
*	This is a simple echo server.  This demonstrates the steps to set up
*	a streaming server.
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>

#include "chatPorts.h"

struct sockaddr_in clients[MAXCLIENTS];
int nClients = 0;

int sameAddr (struct sockaddr_in *a1, struct sockaddr_in *a2) {
  return (a1-> sin_port == a2->sin_port &&
	  a1->sin_addr.s_addr == a2->sin_addr.s_addr);
}

int main(int Count, char *Strings[]) {
  int sockfd;
  struct sockaddr_in self;
  char buffer[MAXBUF];

    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
      perror("Socket");
      exit(errno);
    }

    bzero(&self, sizeof(self));
    self.sin_family = AF_INET;
    self.sin_port = htons(TALKPORT);
    self.sin_addr.s_addr = INADDR_ANY;

    int optval = 1;
    setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if ( bind(sockfd, (struct sockaddr*)&self, sizeof(self)) != 0 ) {
      perror("socket--bind");
      exit(errno);
    }
    
    while (1) {
      int i, j, msgSize;

      struct sockaddr_in client_addr;
      int addrlen = sizeof(client_addr);
		
      // receive a message
      msgSize = recvfrom (sockfd, buffer, sizeof(buffer), 0,
			  (struct sockaddr*) &client_addr, &addrlen);
      printf("%s:%d connected\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
      
      // see if it's a known client
      for (i = 0; i < nClients; i++) 
	if (sameAddr (&client_addr, &clients[i]))
	  break;

      // check for too many clients
      if (i >= MAXCLIENTS) {
	strcpy (buffer, "Max clients exceeded.");
	sendto(sockfd, buffer, strlen (buffer), 0,
	       (struct sockaddr*) &client_addr, addrlen);
	continue;
      }
      // check for new client
      if (i >= nClients) {
	clients[i].sin_addr = client_addr.sin_addr;
	clients[i].sin_port = client_addr.sin_port;
	nClients++;
      }
      // send message to everyone except sender
      for (j = 0; j < nClients; j++)
	if (j != i)
	  sendto(sockfd, buffer, msgSize, 0,
		 (struct sockaddr*) &clients[j], addrlen);
      
    }
    
    /*---Clean up (should never get here!)---*/
    close(sockfd);
    return 0;
}

