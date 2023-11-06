#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc,char **argv)
{
  int sockfd,n;
  char sendline[100];
  char recvline[100];
  struct sockaddr_in servaddr;

  sockfd=socket(AF_INET,SOCK_STREAM,0);
  // azzeramento della variabile servaddr
  bzero(&servaddr,sizeof servaddr);

  servaddr.sin_family=AF_INET;
  servaddr.sin_port=htons(22000);

  inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr)); /*convert IPv4 and IPv6 addresses from text to binary form*/

  if (connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0) {
    printf("Connection Failed \n");
    return -1;
  }

  while(1)
  {
    bzero( sendline, 100);
    bzero( recvline, 100);
    int nums[2];
    scanf("%d",nums);
    scanf("%d",nums+1);
    int result;
    
    write(sockfd,nums,sizeof(nums));
    // sempre bloccante
    read(sockfd,&result,sizeof(result));
    printf("%d",result);
  }

}
