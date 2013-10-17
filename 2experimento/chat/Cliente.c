#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_SIZE    	80

void *receber_mensagem(void *args);
typedef struct {
	int descritor;
	struct  sockaddr_in ladoServ;/* contem dados do servidor 	*/

}argumentos;


int main(int argc,char * argv[]) {
	
	struct  sockaddr_in ladoServ;/* contem dados do servidor 	*/
	int     sd;          	      /* socket descriptor              */
	int     n,k;                  /* num caracteres lidos do servidor */
	argumentos args;
	char    bufout[MAX_SIZE];     /* buffer de dados enviados  */
	pthread_t thread_id;
	/* confere o numero de argumentos passados para o programa */
  	if(argc<3)  {
    	   printf("Para estabelecer comunicacao: %s <ip_do_servidor> <porta_do_servidor>\n", argv[0]);
    	   exit(1);  
	}

	memset((char *)&ladoServ,0,sizeof(ladoServ)); /* limpa estrutura */
	memset((char *)&bufout,0,sizeof(bufout));     /* limpa buffer */
	
	ladoServ.sin_family      = AF_INET; /* config. socket p. TCP/IP*/
	ladoServ.sin_addr.s_addr = inet_addr(argv[1]); // ip do servidor
	ladoServ.sin_port        = htons(atoi(argv[2]));// porta da conexão

	/* Cria socket */
	sd = socket(AF_INET, SOCK_STREAM, 0);
	
	if (sd < 0) {
		fprintf(stderr, "Criacao do socket falhou!\n");
		exit(1); 
	}


	/* Conecta socket ao servidor definido */
	if (connect(sd, (struct sockaddr *)&ladoServ, sizeof(ladoServ)) < 0) {
		fprintf(stderr,"Tentativa de conexao falhou!\n");
		exit(1);
	}
	args.descritor=sd;
	args.ladoServ = ladoServ;
	pthread_create(&thread_id,NULL,&receber_mensagem,(void *)&args);
	while (1) {
		
		printf("> ");
		fgets(bufout, MAX_SIZE, stdin);    /* le dados do teclado */
		send(sd,&bufout,strlen(bufout),0); /* enviando dados ...  */
		if (strncmp(bufout, "FIM",3) == 0) 
			break;
	} /* fim while */
	printf("------- encerrando conexao com o servidor -----\n");
	close (sd);
	pthread_join(thread_id,NULL);
	return (0);
} /* fim do programa */
void *receber_mensagem(void *args){
	argumentos *dados = (argumentos *) args;
	char buffer[MAX_SIZE];
	
	recv(dados->descritor,&buffer,strlen(buffer),0);
	while(1){
		if (strcmp(buffer, "quit.") == 0)	break;
		fprintf(stdout, "[%d:%u] => %s\n", inet_ntoa(dados->ladoServ.sin_addr), ntohs(dados->ladoServ.sin_port), buffer);
	}
	close(dados->descritor);
}