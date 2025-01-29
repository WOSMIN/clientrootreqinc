#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#include <string.h>

int pickChoice(int *sockfd){
	while(1){
		printf("\n 1.Conectare la server");
		printf("\n 2.Deconectare de la server");
		printf("\n 3.Rulare comanda GET/HTTP/1.0\r\n\r\n");
		printf("\n 4.Iesire din aplicatie");
		printf("\n Introduceti optiunea dorita:"); 
	
	int optiune;
	scanf("%d",&optiune);	
	
	bool connected = (*sockfd >= 0);// Asocierea starii conectare/deconectare in functie de valoarea sockfd-ului

	char *msg;
	switch (optiune) {
		
	case 1:{
		if (connected){
			printf("\n Already Connected To The Server \n");
			break;
		}		
		struct addrinfo hints, *res;
		
		if((*sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
			printf("\n Socket Creation Error \n");
			return -1;
		}

		printf("\n SockFd=%d\n",*sockfd);

		memset(&hints , 0, sizeof hints);
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;


		if (getaddrinfo ("www.httpforever.com","80",&hints,&res)<0){
			printf("\n Connection Failed \n");
			return -1;
		}		

		if (connect(*sockfd,res->ai_addr,res->ai_addrlen)<0){
			printf("\n Connection Failed \n");
			return -1;
		}
		
		connected=true;
		break;

	       }//endcase 1

	case 2:{
		if(!connected){
			printf("\n Not Connected To Server \n");
			break;
		}

		printf("\n Deconectare de la server \n");
		close (*sockfd);
		connected = false;
		break;
	       }//endcase2

	case 3:{
		if(!connected){
			printf("\n Please Connect To Server \n");
			break;
		}

		//SEND
		char *msg = "GET / HTTP/1.0\r\n\r\n";

		if(send(*sockfd,msg,strlen(msg),0)<0){
			printf("Can't send data to the server");
			return -1;
		}
	
		else{
			printf("\n Message sent succesfuly \n");
		}
	       
		//RECIVE
		char buf[1025];
		int buflen=sizeof(buf);
		int total_received=0;

		//SCRIERE IN OUTPUT.TXT
		FILE *file = fopen("output.txt", "w");
		if (file == NULL){
			printf("\n Eroare la deschiderea output.txt \n");
			return -1;
		}

		while(1){
		int recvar=recv(*sockfd,buf,buflen-1,0);

			if ( recvar > 0 ){
				total_received += recvar;
				buf[recvar] = '\0'; 
				printf("%s",buf);

				if (fprintf(file, "%s", buf)<0){
					fclose(file);
					printf("Error writing");
					return -1;
				}

			}	
			else if(recvar==0){
				printf("\n Connection closed by remote server \n");
				break;
			}
			else if(recvar <0) {
				printf("\n Failed to recieve from server \n");
				break;
			}

		}
		printf ("\n Bytes recieved %d\n :", total_received);
		close(*sockfd);
		break;
	       }//endcase 3

	case 4:{
		if (connected){
			close(*sockfd);
		}

		 printf("\n Iesire Din Aplicatie \n");
		 close(*sockfd);
		 exit(0);
	 }//endcase 4
	       
	default:
	       printf("\n Optiune Invalida \n");
	       break;
	}
	}		
}


int main(){
	int sockfd=-1;

	pickChoice(&sockfd);

	return 0;}

