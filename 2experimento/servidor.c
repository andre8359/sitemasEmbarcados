#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define QLEN 5          
#define MAX_SIZE 80		

typedef struct {
	int sk;
}argumentos;


void* adiciona_data(void);
void *receber(void *args);
void *enviar(void *args);


int main(int argc, char *argv[]) {
	
	struct sockaddr_in endServ;  
	struct sockaddr_in endCli;  
	int    sk,novo_sk;          
	int     alen; 
	pthread_t thread_0,thread_1;
	argumentos args;

	
	if (argc<3) {
		printf("Digite IP e Porta para este servidor\n");
		exit(1); 
	}

	memset((char *)&endServ,0,sizeof(endServ));

	
	endServ.sin_family 		= AF_INET;           
	endServ.sin_addr.s_addr 	= inet_addr(argv[1]); 	
	endServ.sin_port 		= htons(atoi(argv[2])); 


	sk = socket(AF_INET, SOCK_STREAM, 0);
	if (sk < 0) {
		 fprintf(stderr, "Falha ao criar socket!\n");
		 exit(1); 
	}

	
	if (bind(sk, (struct sockaddr *)&endServ, sizeof(endServ)) < 0) {
		fprintf(stderr,"Ligacao Falhou!\n");
		exit(1);
	 }

	
	if (listen(sk, QLEN) < 0) {
		fprintf(stderr,"Falhou ouvindo porta!\n");
		exit(1);
	 }

	printf("Servidor ouvindo no IP %s, na porta %s ...\n", argv[1], argv[2]);
	
	alen = sizeof(endCli);
	
	if ( (novo_sk=accept(sk, (struct sockaddr *)&endCli,(socklen_t *)&alen)) < 0) {
		fprintf(stdout, "Falha na conexao\n");
		exit(1);
	}
	fprintf(stdout, "Cliente conectado.\n");
	args.sk=novo_sk;


	pthread_create(&thread_0,NULL,&receber,(void*)&args);
	pthread_create(&thread_1,NULL,&enviar,(void*)&args);	

	pthread_join(thread_0,NULL);			
	pthread_join(thread_1,NULL);
	close(sk);
	fprintf(stdout,"Encerrando conexao como cliente\n");
	
} 
void *enviar(void *args){
	char msg[MAX_SIZE];
	argumentos* dados = (argumentos*) args;
	pthread_t thread_id;
	while(1){
		
		memset((char *)&msg,0,sizeof(msg));

		fgets(msg,MAX_SIZE,stdin);

		send(dados->sk, &msg,strlen(msg),0);
		if(strncmp(msg,"quit.",5)==0)	break;
		else{
			pthread_create(&thread_id,NULL,(void*)&adiciona_data,NULL);
			pthread_join(thread_id,NULL);
			printf("Servidor -- ");
			printf("%s\n",msg);
		}	
	}	 
	fprintf(stdout,"Encerrando conexao!!!\n\n");
	close (dados->sk);
	exit(0);
}
void *receber(void *args){
	char bufin[MAX_SIZE];
	argumentos* dados = (argumentos*) args;
	pthread_t thread_id;
	while (1) {
			memset((char *)&bufin,0,sizeof(bufin));

			printf("%d",recv(dados->sk, &bufin, sizeof(bufin),0));

			if (strncmp(bufin, "quit.",5)==0)	break;
			else{
				pthread_create(&thread_id,NULL,(void*)&adiciona_data,NULL);
				pthread_join(thread_id,NULL);
				fprintf(stdout, "Cliente --- %s\n", bufin);	
			}
	} 
	fprintf(stdout,"Encerrando conexao!!!\n\n");
	close (dados->sk);
	exit(0);
}
void* adiciona_data(void)  {
	struct tm *local;
	time_t t;
	t= time(NULL);
	local=localtime(&t);

	printf("%d/%d/%d %dh%dm ",local->tm_mday,local->tm_mon+1,local->tm_year+1900,local->tm_hour,local->tm_min);
} 

