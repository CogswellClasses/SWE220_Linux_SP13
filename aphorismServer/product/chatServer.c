/*
  chat server
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>

#include "chatPorts.h"

int main(int Count, char *Strings[])
{
  int incomingSockFD, outgoingSockFD;
  struct sockaddr_in self, other;
  char buffer[MAXBUF];

  /*---Create datagram sockets---*/
  if ( (incomingSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("incoming Socket");
    exit(errno);
  }

  if ( (outgoingSockFD = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("outgoing Socket");
    exit(errno);
  }
  
  /*---Initialize address/port structures---*/
  bzero(&self, sizeof(self));
  self.sin_family = AF_INET;
  self.sin_port = htons(TALKPORT);
  self.sin_addr.s_addr = INADDR_ANY;

  bzero(&other, sizeof(other));
  other.sin_family = AF_INET;
  other.sin_port = htons(LISTENPORT);

  int optval = 1;
  setsockopt (incomingSockFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  setsockopt (outgoingSockFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
  /*---Assign a port number to the socket---*/
  printf ("bind incoming socket to %s:%d\n",  inet_ntoa(self.sin_addr), ntohs(self.sin_port));

  if ( bind(incomingSockFD, (struct sockaddr*)&self, sizeof(self)) != 0 ) {
    perror("incoming socket--bind");
    exit(errno);
  }

  /*---Forever... ---*/
  while (1) {
    struct sockaddr_in client_addr;
    int addrlen=sizeof(client_addr);
		
    int got = recvfrom (incomingSockFD, buffer, sizeof(buffer), 0,
	      (struct sockaddr*) &client_addr, &addrlen);
    printf("%s:%d connected, got %d '%s'\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), got, buffer);
    
    other.sin_addr = client_addr.sin_addr;
    printf("sending to %s:%d\n", inet_ntoa(other.sin_addr), ntohs(other.sin_port), got);

    sendto(outgoingSockFD, buffer, got, 0,
	   (struct sockaddr*) &other, addrlen);
    
  }
  
  /*---Clean up (should never get here!)---*/
  close(incomingSockFD);
  return 0;
}

