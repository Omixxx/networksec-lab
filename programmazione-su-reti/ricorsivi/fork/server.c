// Lato Server per dimostrare HTTP Server programming con gestione di
//più connessioni ricorsivamente con processi figli

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

#define PORT 2200

int main() {

  int server_fd, newsocket_fd, pid;
  struct sockaddr_in addr;

  /* Creare una socket
  INPUT: dominio -> famiglia di protocolli da utilizzare
         tipo -> tipo di comunicazione (tcp o udp)
         protocollo -> di default tcp/ip oppure non specificato
  OUTPUT: descrittore di file di tipo intero
  */
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(server_fd==-1) {
    printf("Creating socket failed.\n");
    exit(EXIT_FAILURE);
  }

  // Prepara l'indirizzo della socket di ascolto per il bind
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = INADDR_ANY;

  /*Con la bind si lega l'indirizzo alla socket creata
  INPUT: id della socket creata
         indirizzo del server
         lunghezza indirizzo
  OUTPUT: 0 se ha successo
         -1 se c'è un errore
  */
  if( bind(server_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("Bind failed.\n");
    exit(EXIT_FAILURE);
  }
  printf("Bind done.\n");

  /*Il server si mette in ascolto sulla socket creata
  INPUT: id della socket per l'ascolto
         dimensione della backlog
  OUTPUT: 0 se ha successo
         -1 se c'è un errore
  */
  if(listen(server_fd, 10) < 0) {
    printf("Listen error.\n");
    exit(EXIT_FAILURE);
  }
  printf("litening...\n");

  int i = 0;
  while(i < 10) {
  i++;
    /*Crea una nuova socket per la connessione (non può usare quello di ascolto)
    INPUT: id della socket di ascolto
           indirizzo per il salvataggio dell'indirizzo e porta del client
           lunghezza di tale indirizzo
    OUTPUT: descrittore di file della nuova socket se è stata creata
           -1 se non ha avuto successo
    */
    newsocket_fd = accept(server_fd, (struct sockaddr *)NULL, NULL);
    printf("creted new socket\n");

    /*Creo un processo figlio per la gestione della nuova connessione (il padre
      resta in ascolto sulla socket iniziale)
      INPUT: nulla
      OUTPUT: al padre restituisce il pid del figlio
              0 al figlio
    */

    //appena lanciamo la fork il processo figlio viene creato e inizia ad eseguire
    if( (pid = fork()) == 0 ) {
      printf("spawning process:%d\n", getpid());

      /*Il figlio chiude la socket di ascolto in quanto non gli serve
      (comando commentato in quanto causa crash)
      */
      //close(server_fd);
      
      if(newsocket_fd<0) {
        printf("Accept failed.\n");
        exit(EXIT_FAILURE);
      }
      else {
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
        printf("sending message:%s\n",buffer);
        write(newsocket_fd, &buffer, strlen(buffer));
      }
    }

    //Padre chiude socket di connessione
    close(newsocket_fd);
    printf("closed child socket\n");
  }

}
