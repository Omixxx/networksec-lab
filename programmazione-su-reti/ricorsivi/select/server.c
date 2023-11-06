//SELECT - server
//Programma esempio per la gestione di uno o più descrittori contemporaneamente
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
#include <sys/select.h>
#include <sys/time.h>

#define MAX_CONNECTIONS 2
#define BUFF_LEN 30000

int arrmax(int*arr) {
  int i,max = 0;
  for(i=0; i<MAX_CONNECTIONS; i++) {
    if(arr[i]>max) {
      max = arr[i];
    }
  }
  return max;
}

int findfreepos(int*arr) {
  int i=0;
  while(i<MAX_CONNECTIONS && arr[i]>0) {
    i++;
  }

  return i<MAX_CONNECTIONS?i:-1;

}

int max(int a, int b) {
  return a>b?a:b;
}

int getactiveconnections(int*arr) {
  int i,count=0;
  for(i=0; i<MAX_CONNECTIONS; i++) {
    if(arr[i]>0) {
      count++;
    }
  }
  return count;
}


int main(int argc, char **argv) {

  int listen_fd, port, newsocket_fd, max_fd, i, frepos;
  int conn_fds[MAX_CONNECTIONS] = {0};
  struct sockaddr_in addr;
  fd_set  file_descriptor_set;
  char *exit_string = "bye\n";
  char buffer[BUFF_LEN]; //reversed[BUFF_LEN];

  /*Azzera la struttura fdset, ovvero la struttura dei descrittori
  INPUT: locazione della struttura
  OUTPUT: void
  */
  FD_ZERO(&file_descriptor_set);

  //Verifico che oltre al nome dell'eseguibile, sia stata inserita la porta 8080
  if (argc != 2) {
    printf("Errore parametri.\n");
    exit(EXIT_FAILURE);
  }
  port = atoi(argv[1]);

  /* Creare una socket
  INPUT: dominio -> famiglia di protocolli da utilizzare
         tipo -> tipo di comunicazione (tcp o udp)
         protocollo -> di default tcp/ip oppure non specificato
  OUTPUT: descrittore di file di tipo intero
  */
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if(listen_fd==0) {
    printf("Creating socket failed.\n");
    exit(EXIT_FAILURE);
  }

  // Prepara l'indirizzo della socket di ascolto per il bind
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  /*Con la bind si lega l'indirizzo alla socket creata
  INPUT: id della socket creata
         indirizzo del server
         lunghezza indirizzo
  OUTPUT: 0 se ha successo
         -1 se c'è un errore
  */
  if( bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    printf("Bind failed.\n");
    exit(EXIT_FAILURE);
  }
  else {
    printf("Bind done.\n");
  }

  if(listen(listen_fd, 10) < 0) {
    printf("Listen error.\n");
    exit(EXIT_FAILURE);
  }

  while(1) {

    FD_SET(listen_fd, &file_descriptor_set);
    for(i=0; i<MAX_CONNECTIONS; i++) {
      if(conn_fds[i]>0) {
        FD_SET(conn_fds[i], &file_descriptor_set);
      }
    }

    //calcola il massimo descrittore da controllare (fin fove controllare nell'array). Ci serve per 
    //dire alla select fino a che numero di descrittori deve controllare
    max_fd = max(arrmax(conn_fds), listen_fd);

    if( select(max_fd+1, &file_descriptor_set, NULL, NULL, NULL) < 0 ) {
      printf("Errore nella select.\n");
      exit(EXIT_FAILURE);
    }

    if(FD_ISSET(listen_fd, &file_descriptor_set)) {

      newsocket_fd = accept(listen_fd, (struct sockaddr *)NULL, NULL);
      if(newsocket_fd<0) {
        printf("Accept failed.\n");
        exit(EXIT_FAILURE);
      }

      frepos = findfreepos(conn_fds);
      if(frepos<0) {
        printf("Troppe connessioni!\n");
        close(newsocket_fd);
        continue;
      }
      conn_fds[frepos] = newsocket_fd;

      FD_SET(newsocket_fd, &file_descriptor_set);

      printf("Accepted new connection (pos %d).\n", frepos);

      printf("Current active connections: %d\n",getactiveconnections(conn_fds));

    }
    else {

      for(i=0; i<MAX_CONNECTIONS; i++) {

        if(conn_fds[i]<1) {
          continue;
        }

        if( FD_ISSET(conn_fds[i], &file_descriptor_set) ) {

          memset(buffer, '\0', sizeof(buffer));

          int n = read( conn_fds[i] , buffer, BUFF_LEN);

          if(n<1 || strcmp(buffer, exit_string)==0) {
            
            printf("Chiusura socket...\n");
            close(conn_fds[i]);

            FD_CLR(conn_fds[i], &file_descriptor_set);
            conn_fds[i] = 0;
            printf("closing conn %d\n",i);
            break;
          }
          else {
            printf("write back conn %d\n",i);

            int dim = strlen(buffer);

            for (int j = 0;j < (dim/2);j++) {
              char temp = buffer[j];
              buffer[j] = buffer[dim-j-1];
              buffer[dim-j-1] = temp;
            }
            
            printf("Sending %s\n",buffer);
            write(conn_fds[i], &buffer, BUFF_LEN);
          }

        }

      }

    }

  }

  close(listen_fd);


}
