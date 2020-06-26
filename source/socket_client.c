#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <termios.h>


/*
* Here I send a datagram to a receiver whose name I get from the
* command line arguments. The form of the command line is:
* dgramsend hostname portnumber
*/
void main(int argc, char* argv[])
{
  int sock;
  struct sockaddr_in name;
  struct addrinfo *hp;
  struct addrinfo hints;

  //struct hostent *hp, *gethostbyname();
  /* Create socket on which to send. */
  sock = socket(AF_INET,SOCK_DGRAM, 0);
  if (sock == -1) {
    perror("opening datagram socket");
    exit(1);
  }
  /*
  * Construct name, with no wildcards, of the socket to ‘‘send’’
  * to. gethostbyname returns a structure including the network
  * address of the specified host. The port number is taken from
  * the command line.
  */
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  hints.ai_canonname = NULL;
  hints.ai_addr = NULL;
  hints.ai_next = NULL;
  struct sockaddr_in server;

  int res = getaddrinfo(argv[1],argv[2],&hints,&hp);
  if (res != 0) {
    fprintf(stderr, "%s: getting address error\n", argv[1]);
    exit(2);
  }
  int ch = 0;
  char data;
  int n,len;
    char buffer[1024];
    
  struct termios oldtc;
  struct termios newtc;
  tcgetattr(STDIN_FILENO, &oldtc);
  newtc = oldtc;
  newtc.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newtc);

//    system ("/bin/stty raw");
  while ( ( ch = getchar()) != 'q') {
    /* Send message. */
    // printf("\nSending Message %c\n", ch);
    data = (char) ch;
    printf("\nSending Message %c\n", data);
    if (sizeof(data) && sendto(sock, &data, sizeof(data) ,0, hp->ai_addr,hp->ai_addrlen) == -1)
        perror("sending datagram message");
    //receiving a message from the server...
    // n = recvfrom(sock, (char *)buffer, 1024,
    //                 MSG_WAITALL, (struct sockaddr *) &server,
    //                 &len);
    //     buffer[n] = '\0';
    //     printf("Server : %s\n", buffer);
  }
  tcsetattr(STDIN_FILENO, TCSANOW, &oldtc);
  fflush(stdout);

//    system ("/bin/stty cooked");
  close(sock);
  exit(0);
}