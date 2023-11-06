#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 1112

int main(int argc,char **argv) {
  int sockfd,n;
  char sendline[30000];
  char recvline[30000];
  struct sockaddr_in servaddr;

  sockfd=socket(AF_INET,SOCK_STREAM,0);
  // azzeramento della struttura degli indirizzi
  bzero(&servaddr,sizeof servaddr);

  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(PORT);

  inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr)); /*convert IPv4 and IPv6 addresses from text to binary form*/

  if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) {
    printf("Connection Failed \n");
    return -1;
  }
  
  while(1) {
    bzero( sendline, 30000);
    bzero( recvline, 30000);
    scanf("%s",sendline);
    
    
    write(sockfd,sendline,30000);
    // sempre bloccante
    read(sockfd,recvline,30000);
    printf("%s\n",recvline);
  }
}
