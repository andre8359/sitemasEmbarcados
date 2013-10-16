#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaforo.h"
#include "engine.h"

#define DEBUG 0
#define PODE_CRIAR_FILHO 1
#define NAO_PODE_CRIAR_FILHO 0

#define SEMAFORO_KEY 0x1234
#define TOTAL_SEMAFOROS 5

#define PONTE 0
#define NUM_CRIANCAS_DIREITO 1
#define NUM_CRIANCAS_ESQUERDO 2
#define MUTEX_DIREITO 3
#define MUTEX_ESQUERDO 4

#define LADO_DIREITO 1
#define LADO_ESQUERDO -1

int semaforos;

int crianca_id,tempo_decisao, lado, numeroTravessias;
char lado_da_ponte[9], estado[13];

int gerarNumeroAleatorio(int, int);
void inicarValoresDoSemaforo(int);
void imprimirCrianca();

int definirLado(int);
int inverterLado(int);

void tomarCoragem();
void realizarTravessiaDaPonte();
void crianca();

int main()
{
	int i, stats;
	int criarFilho = PODE_CRIAR_FILHO;
	int numeroCriancas = gerarNumeroAleatorio(3, 7);
	if(DEBUG)
		numeroCriancas = 2;
		
	pid_t cpid[numeroCriancas];

    semaforos = criar_semaforo(SEMAFORO_KEY, TOTAL_SEMAFOROS);
    inicarValoresDoSemaforo(semaforos);
    
    cabecalho_da_tabela();
    
	for(i = 0; i < numeroCriancas; i++)
	{
		if(criarFilho == PODE_CRIAR_FILHO)
			cpid[i] = fork();
		if(!cpid[i])
		{
    	   criarFilho = NAO_PODE_CRIAR_FILHO;
    	   crianca_id = i;    
           crianca();   
           exit(0);
		}
	}
	
	for(i = 0; i < numeroCriancas; i++)
		waitpid(cpid[i], &stats, 0);
	
	deletar_semaforo(semaforos, 0);
	return 0;	
}

void crianca()
{
	lado = definirLado(gerarNumeroAleatorio(0, 2));
	numeroTravessias = gerarNumeroAleatorio(1, 4);
	if(DEBUG)
		numeroTravessias = 2;
		
	while(numeroTravessias)
	{
		tomarCoragem();
		
		if(lado == LADO_ESQUERDO)
		{
			down(semaforos, MUTEX_ESQUERDO);
			up(semaforos, NUM_CRIANCAS_ESQUERDO);
			if(getValorSemaforo(semaforos, NUM_CRIANCAS_ESQUERDO) == 1)
				down(semaforos, PONTE);

			up(semaforos, MUTEX_ESQUERDO);
			
			realizarTravessiaDaPonte();
			
			down(semaforos, MUTEX_ESQUERDO);
			down(semaforos, NUM_CRIANCAS_ESQUERDO);
			if(getValorSemaforo(semaforos, NUM_CRIANCAS_ESQUERDO) == 0)
				up(semaforos, PONTE);
			up(semaforos, MUTEX_ESQUERDO);
		}
		else
		{
			down(semaforos, MUTEX_DIREITO);
			up(semaforos, NUM_CRIANCAS_DIREITO);
			if(getValorSemaforo(semaforos, NUM_CRIANCAS_DIREITO) == 1)
				down(semaforos, PONTE);

			up(semaforos, MUTEX_DIREITO);
			realizarTravessiaDaPonte();
			
			down(semaforos, MUTEX_DIREITO);
			down(semaforos, NUM_CRIANCAS_DIREITO);
			if(getValorSemaforo(semaforos, NUM_CRIANCAS_DIREITO) == 0)
				up(semaforos, PONTE);
			up(semaforos, MUTEX_DIREITO);
		}
		
		lado = inverterLado(lado);
		definirLado(lado);
		numeroTravessias--;
		imprimirCrianca();
	}
}

void tomarCoragem()
{
	strcpy(estado, "Decidindo");
	tempo_decisao = gerarNumeroAleatorio(5, 9);
	if(DEBUG)
		tempo_decisao = gerarNumeroAleatorio(2, 5);
	imprimirCrianca();
	sleep(tempo_decisao);
}

void realizarTravessiaDaPonte()
{
	strcpy(estado, "Atravessando");
	imprimirCrianca();
	sleep(3);
	strcpy(estado, "Cheguei");
}

int definirLado(int ladoParaVerificar)
{
	if(ladoParaVerificar == LADO_DIREITO)
		strcpy(lado_da_ponte, "Direito");
    else 
    {
        ladoParaVerificar = LADO_ESQUERDO;
		strcpy(lado_da_ponte, "Esquerdo"); 
    }
    return ladoParaVerificar;
}

int inverterLado(int ladoParaSerInvertido)
{
	return ladoParaSerInvertido*-1;
}


void inicarValoresDoSemaforo(int semid)
{
    iniciarSemaforo(semid, 1, PONTE);
    iniciarSemaforo(semid, 0, NUM_CRIANCAS_ESQUERDO);
    iniciarSemaforo(semid, 0, NUM_CRIANCAS_DIREITO);
    iniciarSemaforo(semid, 1, MUTEX_DIREITO);
    iniciarSemaforo(semid, 1, MUTEX_ESQUERDO);
}    

int gerarNumeroAleatorio(int inicio, int fim)
{	
	int randomNumber;
	srand(time(0)+getpid());
	randomNumber = random() % fim;
	return randomNumber ? randomNumber : randomNumber + inicio;
}

void imprimirCrianca()
{
	printCrianca(crianca_id, numeroTravessias, lado_da_ponte, tempo_decisao, estado);
}
