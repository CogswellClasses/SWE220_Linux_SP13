/*****************************************************************************/
/*** simple-client.c                                                       ***/
/***                                                                       ***/
/*****************************************************************************/

/**************************************************************************
*   This is a simple client socket reader.  It opens a socket, connects
*   to a server, reads the message, and closes.
**************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include "chatPorts.h"

int main(int argc, char *argv[])
{
  int sockfd;
  struct sockaddr_in dest, serverAddr;
  int serverAddrLen;
  char buffer[MAXBUF];
  char *msgStart;
  char *server_name, *nickname;
  int nicknameLength;
  pid_t child;
  
  struct hostent *he;
  struct in_addr **addr_list;

  if (argc != 3) {
    fprintf (stderr, "usage: hostName nickName\n");
    exit (EXIT_FAILURE);
  }

  server_name = argv[1];
  nickname = argv[2];
  strcpy (buffer, nickname);
  nicknameLength = strlen (nickname) + 1;
  msgStart = buffer + nicknameLength;

  if ( (he = gethostbyname( server_name ) ) == NULL) {
    fprintf(stderr, "gethostbyname (%s)returned nothing.\n", server_name);
    exit (EXIT_FAILURE);
  }

  addr_list = (struct in_addr **) he->h_addr_list;
  if (addr_list[0] == NULL) {
    perror ("no results");
    exit (EXIT_FAILURE);
  }

  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
    perror("Socket");
    exit(errno);
  }

  bzero(&dest, sizeof(dest));
  dest.sin_family = AF_INET;
  dest.sin_port = htons(TALKPORT);
  dest.sin_addr = *addr_list[0];
    
  strcpy (msgStart, "Entered chat room\n");
  sendto(sockfd, buffer, nicknameLength + strlen(msgStart) + 1, 0, (struct sockaddr *)&dest, sizeof(dest));
    
  child = fork ();
  if (child < 0) {
    perror ("cannot fork");
    exit (EXIT_FAILURE);
  }
  if (child) { // parent does the talking
    while (1) {
      int n = MAXBUF - nicknameLength;
      int got = getline (&msgStart, &n, stdin);
      if (got < 0) break;
      msgStart[got++] = 0;
      sendto(sockfd, buffer, nicknameLength + got, 0, (struct sockaddr *)&dest, sizeof(dest));
    }
  } else { // child does the listening
    while (1) {
      serverAddrLen = sizeof (serverAddr);
      recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *) &serverAddr, &serverAddrLen);
      printf ("%s says:\n%s\n", buffer, buffer + strlen(buffer) + 1);
    }
  }
  
  /*---Clean up---*/
  close(sockfd);
  return 0;
}


