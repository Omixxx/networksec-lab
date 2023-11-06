//SELECT - client
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
#include <sys/select.h>
#include <sys/time.h>

#define MAXLINE 1000
#define MAX(a,b)\
({a>b?:b;})

void dg_cli_echo_sel(FILE *fd, int sockd) {

	int n, maxd;
	char sendline[MAXLINE], recvline[MAXLINE + 1];
	char buff[MAXLINE], str[MAXLINE], port[MAXLINE];
	fd_set  rset;
	char *serv_says = "Server says:";

	buff[0] = '\0';

	/*Azzera la struttura fdset, ovvero la struttura dei descrittori
	INPUT: locazione della struttura
	OUTPUT: void
	*/
	FD_ZERO(&rset);

	printf("Ready to send.\n");

	while(1) {

		/*Registro il descrittore, ovvero mette a 1 il bit del descrittore di file
		passato in input (lo inserisce nel gruppo fd_set)
		INPUT: id della socket
					 posizione della struttura
		OUTPUT: void
		*/
		FD_SET(fileno(fd), &rset);  //fileno ritorna il descrittore di file dello stdin
		FD_SET(sockd, &rset);

		//calcolo il massimo descrittore da controllare (fin dove controllare nell'array)
		maxd = MAX(fileno(fd), sockd) + 1;

		//Inizializzo i buffer per dati in lettura e scrittura a zero
		memset(recvline, '\0', sizeof(recvline));
		memset(sendline, '\0', sizeof(sendline));

		/*Si mette in attesa che qualche descrittore di file sia pronto. Permette di controllare
		contemporaneamente uno o più descrittori (per lettura [stdin, socket con server],
		per scrittura o gestione errori)
		INPUT: numero massimo di descrittori controllati da select
					 indirizzo della struttura con tutti i descrittori per lettura
					 indirizzo della struttura con tutti i descrittori per scrittura
					 indirizzo della struttura con tutti i descrittori per errori
					 val max che la funzione attende per individuare un descrittore pronto
		*/
		if( select(maxd, &rset, NULL, NULL, NULL) < 0 ) {
			printf("Errore nella select.\n");
			exit(EXIT_FAILURE);
		}

		/*Per capire se il descrittore è attivo o no (se fa parte della struttura fd_set)
		verifica se il bit relativo al fd è 1 (nel mio caso se la socket con server è pronta)
		INPUT: id della socket
					 posizione della struttura
		OUTPUT: != 0 se ne fa parte
						0 se non ne fa parte
		*/
		if(FD_ISSET(sockd, &rset)) {

			/*Per leggere dalla socket
			INPUT: id della socket dalla quale leggere
						 buffer dove salvare i dati
						 dimensione del buffer
			OUTPUT: byte letti
							-1 se c'è un errore
			*/
			if ( (n = read(sockd, recvline, MAXLINE)) < 0) {
				printf("Errore lettura socket.\n");
				exit(EXIT_FAILURE);
			}

			//se non ci sono dati in lettura, la chat termina
			if (n == 0) {
				printf("Conversazione terminata.\n");
				exit(EXIT_FAILURE);
			}

			/*Scrivo a linea di comando i dati letti (se non sono uscito al controllo precedente)
			fino all'end of file
			*/
			if( fputs(recvline, stdout) == EOF ) {
				printf("errore nella fputs\n");
				exit(EXIT_FAILURE);
			}

		}

		/*Per capire se il descrittore è attivo o no (se fa parte della struttura fd_set)
		verifica se il bit relativo al fd è 1 (nel mio caso se la lettura da stdinput è pronta)
		INPUT: id della socket
					 posizione della struttura
		OUTPUT: != 0 se ne fa parte
						0 se non ne fa parte
		*/
		if( FD_ISSET(fileno(fd), &rset) ) {

			//Funzione per leggere da linea di comando
			if( fgets(sendline, MAXLINE, fd) == NULL) {
				printf("Ricevuto EOF\n");
				/*if( (write(sockd, "bye\n", strlen("bye\n"))) < 0) {
					printf("errore chiusura server");
					exit(EXIT_FAILURE);
				}*/
				close(sockd);
				return;
			}

			/*Per scrivere sulla socket le info ricevute in input
			INPUT: id della socket sulla quale scrivere
						 buffer con i dati da scrivere
						 lunghezza del buffer
			OUTPUT: numero di bytes scritti
						 -1 se c'è un errore
			*/
			if( (write(sockd, sendline, strlen(sendline))) < 0) {
				printf("errore nella write");
				exit(EXIT_FAILURE);
			}

		}


	}

}


int main(int argc, char **argv) {

	int sockd, n, porta;
	struct sockaddr_in servaddr;

	//verifica che gli input siano tre (./<file da esguire> "127.0.0.1" 8080)
	if (argc != 3) {
		printf("Errore parametri.\n");
		exit(EXIT_FAILURE);
	}

	/* Creare una socket
  INPUT: dominio -> famiglia di protocolli da utilizzare
         tipo -> tipo di comunicazione (tcp o udp)
         protocollo -> di default tcp/ip oppure non specificato
  OUTPUT: descrittore di file di tipo intero
  */
	if ( (sockd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("Errore socket.\n");
		exit(EXIT_FAILURE);
	}

	// Prepara l'indirizzo della socket di ascolto per il bind
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(argv[1]);
  porta = atoi(argv[2]);
	servaddr.sin_port = htons(porta);

	/*CLient contatta il server per stabilire una connessione
	INPUT: id della socket
				 indirizzo del server
				 dimensione di tale indirizzo
	OUTPUT: 0 se ha avuto successo
				 -1 se c'è stato un errore
	*/
	if (connect(sockd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
	else
		printf("connected to the server..\n");

	//Chiamo la funzione per lo scambio di messaggi
	dg_cli_echo_sel(stdin, sockd);


	exit(0);

}
