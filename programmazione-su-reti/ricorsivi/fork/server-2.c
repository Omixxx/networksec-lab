#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(int argc,char **argv) {

  char str[100];
  int listen_fd, conn_fd, pid;

  struct sockaddr_in servaddr;

  listen_fd = socket(AF_INET, SOCK_STREAM, 0);

  bzero( &servaddr, sizeof(servaddr));

  // indirizzo del server con annessa porta di ascolto
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htons(INADDR_ANY);
  servaddr.sin_port = htons(2200);

  // lega la socket di ascolto all'indirizzo del server e alla porta
  if(bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
    printf("Bind failed.\n");
    exit(EXIT_FAILURE);
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
    /*Crea una nuova socket per la connessione (non può usare quello di ascolto)
    INPUT: id della socket di ascolto
           indirizzo per il salvataggio dell'indirizzo e porta del client
           lunghezza di tale indirizzo
    OUTPUT: descrittore di file della nuova socket se è stata creata
           -1 se non ha avuto successo
    */
    int  newsocket_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);

    /*Creo un processo figlio per la gestione della nuova connessione (il padre
      resta in ascolto sulla socket iniziale)
      INPUT: nulla
      OUTPUT: al padre restituisce il pid del figlio
              0 al figlio
    */

    //appena lanciamo la fork il processo figlio viene creato e inizia ad eseguire
    printf("AA");
    if( ( pid = fork()) == 0 ) {

      printf("BB");
      /*Il figlio chiude la socket di ascolto in quanto non gli serve
      (comando commentato in quanto causa crash)
      */
      //close(server_fd);

      if(newsocket_fd<0) {
        printf("Accept failed.\n");
        exit(EXIT_FAILURE);
      }
      else {
        printf("ciao");
        //Per leggere dal client ho bisogno di una struttura per salvare i dati in input
        char buffer[30000] = {0};

        /*Per leggere dalla socket di scambio dati ( non quella di ascolto)
        INPUT: socket dalla quale leggere i dati
               buffer per salvare i dati letti
               dimensione del buffer
        OUTPUT: numero dei caratteri letti
                -1 se c'è stato un errore
        */
        read( newsocket_fd , buffer, 30000);

        int dim = strlen(buffer);  
        printf("%d", dim);

        // Inverto la stringa del client
        for (int i = 0;i < (dim/2);i++) {
          char temp = buffer[i];
          buffer[i] = buffer[dim-i-1];
          buffer[dim-i-1] = temp;
        }

        //Prendo il pid del processo figlio
        //pid_t childpid = getpid();


        /*Per scrivere sulla socket
        INPUT: id della socket sulla quale scrivere
               buffer con i dati da scrivere
               lunghezza del buffer
        OUTPUT: numero di bytes scritti
               -1 se c'è un errore
        */
        write(newsocket_fd,buffer , strlen(buffer));

      }
    }

    //Padre chiude socket di connessione
    close(newsocket_fd);
  }
}
