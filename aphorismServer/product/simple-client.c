/* simple-client.c
 *
 * Copyright (c) 2000 Sean Walton and Macmillan Publishers.  Use may be in
 * whole or in part in accordance to the General Public License (GPL).
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
*/

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

#define PORT       9999
#define SERVER_NAME     "hydra.cpc.local"
#define MAXBUF          1024

int main()
{   int sockfd;
    struct sockaddr_in dest;
    char buffer[MAXBUF];

    struct hostent *he;
    struct in_addr **addr_list;

    if ( (he = gethostbyname( SERVER_NAME ) ) == NULL)
    {
      fprintf(stderr, "gethostbyname (%s)returned nothing.\n", SERVER_NAME);
	exit (EXIT_FAILURE);
    }
    addr_list = (struct in_addr **) he->h_addr_list;
    if (addr_list[0] == NULL) {
      perror ("no results");
      exit (EXIT_FAILURE);
    }

    /*---Open socket for streaming---*/
    if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 )
    {
        perror("Socket");
        exit(errno);
    }

    /*---Initialize server address/port struct---*/
    bzero(&dest, sizeof(dest));
    dest.sin_family = AF_INET;
    dest.sin_port = htons(PORT);
    dest.sin_addr = *addr_list[0];
    
    /*---Connect to server---*/
    if ( connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0 )
    {
      perror("Connect ");
      exit(errno);
    }

    /*---Get whatever server sends"---*/
    bzero(buffer, MAXBUF);
    recv(sockfd, buffer, sizeof(buffer), 0);
    printf("Received: %s", buffer);

    /*---Clean up---*/
    close(sockfd);
    return 0;
}


