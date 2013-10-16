#include "semaforo.h"
#include <stdio.h>
#include <stdlib.h>

#define ERRO -1
#define SUCESSO 0

int criarChave(int identificador)
{
	return ftok("semaforo.c", identificador);
}

int criar_semaforo(key_t key, int numeroDeSemaforos)
{

	int semid;

	semid = semget(key,numeroDeSemaforos, IPC_CREAT | 0666);

	if(semid == ERRO)
	{
		fprintf(stderr, "Erro ao cria semaforo\n");
		exit(-1);
	}


	return semid;

}

int iniciarSemaforo(int semid, int valorInicial, int numeroDoSemaforo)
{
	union semun
	{
		int val;
		struct semid_ds *buf;
		ushort array[1];
	}arg_ctl;

	arg_ctl.val = valorInicial;

	if (semctl(semid,numeroDoSemaforo,SETVAL,arg_ctl) == -1)
	{
		fprintf(stderr, "Erro ao iniciar o semaforo\n");
		exit(-2);
	}
}


int down(int identificadorDoSemaforo, int numeroDoSemaforo)
{

	struct sembuf sempar[1];
	sempar[0].sem_num = numeroDoSemaforo;
	sempar[0].sem_op = -1;
	sempar[0].sem_flg = SEM_UNDO;

	if(semop(identificadorDoSemaforo,sempar,1) == ERRO)
	{
		fprintf(stderr, "Erro ao realizar operacao down\n");
		exit(ERRO);
	} 

	return SUCESSO;
}

int up(int identificadorDoSemaforo, int numeroDoSemaforo)
{
	struct sembuf sempar[0];
	sempar[0].sem_num = numeroDoSemaforo;
	sempar[0].sem_op = 1;
	sempar[0].sem_flg = SEM_UNDO;

	if(semop(identificadorDoSemaforo, sempar, 1) == ERRO)
	{
		fprintf(stderr, "Erro ao realizar operacao up\n");
		exit(ERRO);
	}	

	return SUCESSO;
}

int deletar_semaforo(int identificadorDoSemaforo, int numeroDoSemaforo)
{

	if(semctl(identificadorDoSemaforo, numeroDoSemaforo, IPC_RMID, 0) == -1)
	{
		fprintf(stderr, "Erro ao deletar semaforo\n");
		exit(ERRO);
	}

	return SUCESSO;
}

int getValorSemaforo(int identificadorDoSemaforo, int numeroDoSemaforo)
{
    return semctl(identificadorDoSemaforo, numeroDoSemaforo, GETVAL, 0);
}
