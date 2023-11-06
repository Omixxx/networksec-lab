#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc,char **argv) {

  char str[100];
  int listen_fd, conn_fd;

  struct sockaddr_in servaddr;

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero( &servaddr, sizeof(servaddr));

  // indirizzo del server con annessa porta di ascolto
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(22000);

  // lega la socket di ascolto all'indirizzo del server e alla porta
  if( bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr))< 0) {
    printf("errore in bind");
    return 1;
  }

  // mette in ascolto la socket di ascolto(non bloccante)
  listen(listen_fd, 10);

  // resta in attesa di una richiesta di connessione da parte di un client
  // non passiamo la struttura del client perchè non ci interessa salvare le sue informazioni (indirizzo e porta)
  // normalmente la struttura viene usata per salvare l'indirizzo di chi si connette
  // la funzione accept() comunque lega la nuova socket di connessione all'indirizzo del client tuttavia non salviamo le informazioni nella struttura
  // visto che non le riutilizzeremo
  conn_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

  while(1)
  {
    int nums[2];

    // sempre bloccante
    read(conn_fd,nums,sizeof(nums));

    int sum = nums[0] + nums[1];

    write(conn_fd, &sum, sizeof(sum));
  }
}
