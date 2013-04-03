/*
  chat client: talk to chat room
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "chatPorts.h"


int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in dest, serverAddr;
  int serverAddrLen;
  char *buf = 0;
  char buffer[MAXBUF+1];
  char *server_name;

  struct hostent *he;
  struct in_addr **addr_list;

  if (argc != 2) {
    fprintf (stderr, "need a host name as the only argument\n");
    exit (EXIT_FAILURE);
  }
  server_name = argv[1];

  if ( (he = gethostbyname( server_name ) ) == NULL) {
      fprintf(stderr, "gethostbyname (%s)returned nothing.\n", server_name);
      exit (EXIT_FAILURE);
    }
  addr_list = (struct in_addr **) he->h_addr_list;
  if (addr_list[0] == NULL) {
    perror ("no results");
    exit (EXIT_FAILURE);
  }

  /*---Open socket for datagram---*/
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
      perror("Socket");
      exit(errno);
    }

    /*---Initialize server address/port struct---*/
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(TALKPORT);
    dest.sin_addr = *addr_list[0];

    // send the user's input lines to the server
    bzero(buffer, MAXBUF);
    while (1) {
      size_t n = MAXBUF;
      int got = getline (&buf, &n, stdin);
      if (got < 0) break;
      buf[got] = 0;
      got++;
      printf ("send to %s:%d %s\n",  inet_ntoa(dest.sin_addr), ntohs(dest.sin_port), buf);
      sendto (sockfd, buf, got, 0, (struct sockaddr *)&dest, sizeof(dest));
      printf ("sent %d bytes\n", got);
    }

    /*---Clean up---*/
    close(sockfd);
    return 0;
}


