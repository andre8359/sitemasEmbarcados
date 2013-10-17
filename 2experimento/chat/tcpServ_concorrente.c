#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#define QLEN            5               /* tamanho da fila de clientes  */
#define MAX_SIZE	80		/* tamanho do buffer */


/*Argumentos da thread !!*/
typedef struct {
	int descritor;
	struct sockaddr_in endCli;
}argumentos;	

/*Prototipo de atende*/
void *atende_cliente(void *args);



int main(int argc, char *argv[]) {
	
	struct sockaddr_in endServ;  /* endereço do servidor   */
	struct sockaddr_in endCli;   /* endereço do cliente    */
	int    sk, novo_sk;          /* socket descriptors */
	int    pid, alen,n; 
	pthread_t thread_id;
	argumentos args;
	if (argc<3) {
		printf("Digite IP e Porta para este servidor\n");
		exit(1); 
	}

	memset((char *)&endServ,0,sizeof(endServ)); /* limpa variavel endServ    */
	memset((char *)&args,0,sizeof(args)); /* limpa variavel args   */
	endServ.sin_family 		= AF_INET;           	/* familia TCP/IP   */
	endServ.sin_addr.s_addr 	= inet_addr(argv[1]); 	/* endereço IP      */
	endServ.sin_port 		= htons(atoi(argv[2])); /* PORTA	    */

	/* Cria socket */
	sk = socket(AF_INET, SOCK_STREAM, 0);
	if (sk < 0) {
		 fprintf(stderr, "Falha ao criar socket!\n");
		 exit(1); 
	}

	/* liga socket a porta e ip */
	if (bind(sk, (struct sockaddr *)&endServ, sizeof(endServ)) < 0) {
		fprintf(stderr,"Ligacao Falhou!\n");
		exit(1);
	 }

	/* Ouve porta */
	if (listen(sk, QLEN) < 0) {
		fprintf(stderr,"Falhou ouvindo porta!\n");
		exit(1);
	 }

	printf("Servidor ouvindo no IP %s, na porta %s ...\n\n", argv[1], argv[2]);
	/* Aceita conexoes */
	alen = sizeof(endCli);
	for ( ; ; ) {
		 /* espera nova conexao de um processo cliente ... */	
		if ( (novo_sk=accept(sk, (struct sockaddr *)&endCli, &alen)) < 0) {
			fprintf(stdout, "Falha na conexao\n");
			exit(1);
		}
		else{
			args.descritor=novo_sk;
			args.endCli= endCli;
			pthread_create(&thread_id,NULL,&atende_cliente,(void *)&args);
			pthread_join(thread_id,NULL);			
			close (sk); /* encerra o socket sk */
			fprintf(stdout, "Cliente %d: %u conectado.\n", inet_ntoa(endCli.sin_addr), ntohs(endCli.sin_port)); 
			atende_cliente(novo_sk, endCli);
			exit(0);
		} /* fim else-if */
		/* processo pai */
		
		} /* fim for */
} /* fim do programa */

void* atende_cliente(void *args)  {
	
	argumentos* dados = (argumentos*) args; 
	char bufin[MAX_SIZE];
	int  n;

	while (1) {
		memset(&bufin, 0x0, sizeof(bufin));
		n = recv(dados->descritor, &bufin, sizeof(bufin),0);
		if (strncmp(bufin, "quit.", 3) == 0)break;
		fprintf(stdout, "[%d:%u] => %s\n", inet_ntoa(dados->endCli.sin_addr), ntohs(dados->endCli.sin_port), bufin);
	} /* fim while */
	
	fprintf(stdout, "Encerrando conexao com %d:%u ...\n", inet_ntoa(dados->endCli.sin_addr), ntohs(dados->endCli.sin_port));
	close (dados->descritor);
} /* fim atende_cliente */


