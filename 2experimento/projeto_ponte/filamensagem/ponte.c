#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "mensagem.h"
#include "engine.h"

#define SINAL_DE_PERMISSAO_PARA_ATRAVESSAR 23
#define PODE_CRIAR_FILHO 1
#define NAO_PODE_CRIAR_FILHO 0

#define FILA_DIREITA_KEY 0x1234
#define FILA_ESQUERDA_KEY 0x4321

#define LADO_DIREITO 1
#define LADO_ESQUERDO -1

int filaDireita, filaEsquerda;
int numeroCriancas, tempoTravessia;

int crianca_id, tempo_decisao, lado, numeroTravessias;
char lado_da_ponte[9], estado[13];

int gerarNumeroAleatorio(int, int);
void imprimirCrianca();
int extrairPidDaMensagem(buf);
int extrairTempoDeEsperaDaMensagem(buf);
int extrairNumeroTravessiasDaMensagem(buf);
buf criarMensagem();

int definirLado(int);
int inverterLado(int);

void crianca();
void ponte();

void realizarTravessiaDaPonte(int);
void tomarCoragem();
void esperarSinalParaRealizarTravessia();

int main()
{
	int i, criarFilho = PODE_CRIAR_FILHO, stats;

	numeroCriancas = gerarNumeroAleatorio(3,7);

	pid_t cpid[numeroCriancas];

	filaDireita = gerarIdentificadorDaFila(FILA_DIREITA_KEY);
	filaEsquerda = gerarIdentificadorDaFila(FILA_ESQUERDA_KEY);

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
	
	ponte();

	for(i = 0; i < numeroCriancas; i++)
		waitpid(cpid[i], &stats, 0);
	
	removerFilaDeMensagem(filaDireita);
	removerFilaDeMensagem(filaEsquerda);
	return 0;	
}

void crianca()
{
	buf mensagemParaPonte;
	signal(SINAL_DE_PERMISSAO_PARA_ATRAVESSAR, realizarTravessiaDaPonte);
	
	lado = definirLado(gerarNumeroAleatorio(0, 2));
	numeroTravessias = gerarNumeroAleatorio(1, 4);

	while(numeroTravessias)
	{
		tomarCoragem();
		tempoTravessia = gerarNumeroAleatorio(2, 4);

		mensagemParaPonte = criarMensagem();
	    if(lado == LADO_DIREITO)
	       	mandarMensagemParaFila(filaDireita, &mensagemParaPonte);
		else
		   	mandarMensagemParaFila(filaEsquerda, &mensagemParaPonte);
			
		esperarSinalParaRealizarTravessia();
		
		lado = inverterLado(lado);
		definirLado(lado);
		numeroTravessias-=1;
		imprimirCrianca();
	}
}

void ponte()
{
	buf mensagemRecebidaDaCrianca;
	int contadorDeProcessos = 0;
	int esperaDaUltimaCrianca = 0;
	
	while(contadorDeProcessos < numeroCriancas)
	{
		while (receberMensagemDaFila(filaDireita, &mensagemRecebidaDaCrianca) >= 0)
		{
			kill(extrairPidDaMensagem(mensagemRecebidaDaCrianca), SINAL_DE_PERMISSAO_PARA_ATRAVESSAR);
			
			esperaDaUltimaCrianca = extrairTempoDeEsperaDaMensagem(mensagemRecebidaDaCrianca);
			
			if(extrairNumeroTravessiasDaMensagem(mensagemRecebidaDaCrianca) == 1)
				contadorDeProcessos++;
		}
		sleep(esperaDaUltimaCrianca);
		
		while (receberMensagemDaFila(filaEsquerda, &mensagemRecebidaDaCrianca) >= 0)
		{	
			kill(extrairPidDaMensagem(mensagemRecebidaDaCrianca), SINAL_DE_PERMISSAO_PARA_ATRAVESSAR);
			
			esperaDaUltimaCrianca = extrairTempoDeEsperaDaMensagem(mensagemRecebidaDaCrianca);
			
			if(extrairNumeroTravessiasDaMensagem(mensagemRecebidaDaCrianca) == 1)
				contadorDeProcessos++;
		}
		sleep(esperaDaUltimaCrianca);
	}
}


void realizarTravessiaDaPonte(int signalDeAtivacao)
{
	strcpy(estado, "Atravessando");
	imprimirCrianca();
	sleep(tempoTravessia);
	strcpy(estado, "Cheguei");
}

void tomarCoragem()
{
	strcpy(estado, "Decidindo");
	tempo_decisao = gerarNumeroAleatorio(5, 9);
	imprimirCrianca();
	sleep(tempo_decisao);
}

void esperarSinalParaRealizarTravessia()
{
	pause();
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


int extrairPidDaMensagem(buf mensagem)
{
	int pid = 0;
	sscanf(mensagem.mtext, "%d", &pid);
	return pid;
}

int extrairTempoDeEsperaDaMensagem(buf mensagem)
{
	int pid, espera = 0;
	sscanf(mensagem.mtext, "%d %d", &pid, &espera);
	return espera;
}

int extrairNumeroTravessiasDaMensagem(buf mensagem)
{
	int pid, espera, numero = 0;
	sscanf(mensagem.mtext, "%d %d %d", &pid, &espera, &numero);
	return numero;
}

buf criarMensagem()
{
	char text[15];
	buf mensagem;
	mensagem.mtype = 1;
	
	sprintf(text, "%d %d %d", getpid(), tempoTravessia, numeroTravessias);

	strcpy(mensagem.mtext, text);
	return mensagem;
}

int gerarNumeroAleatorio(int inicio, int fim)
{	
	int randomNumber;
	srand(time(0) + getpid());
	randomNumber = rand() % fim;
	return randomNumber ? randomNumber : randomNumber + inicio;
}

void imprimirCrianca()
{
	printCrianca(crianca_id, numeroTravessias, lado_da_ponte, tempo_decisao, estado);
}

