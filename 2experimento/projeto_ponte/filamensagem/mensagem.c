#include <stdlib.h>
#include <stdio.h>
#include "mensagem.h"

int gerarIdentificadorDaFila(key_t keyParaFilaDeMensagem)
{
	int identificadorDaFila;

	if((identificadorDaFila=msgget(keyParaFilaDeMensagem,  0644 | IPC_CREAT)) == -1)
	{
		fprintf(stderr, "Erro ao criar fila de mensagem\n");
		exit(0);
	}

	return identificadorDaFila;
}

int receberMensagemDaFila(int identificadorDaFila, struct my_msgbuf* buf)
{
	int returncode = 0;
	returncode = msgrcv( identificadorDaFila, (struct msgbuf *) buf, sizeof(*buf), 0, IPC_NOWAIT);
	return returncode;
}

void mandarMensagemParaFila(int identificadorDaFila, struct my_msgbuf* buf)
{
	if(msgsnd(identificadorDaFila, (struct msgbuf*) buf, sizeof(*buf), 0) == -1)
	{
		fprintf(stderr, "Erro ao enviar mensagem para fila de mensagens\n");
		exit(0);
	}
}

void removerFilaDeMensagem(int identificadorDaFila)
{
	if(msgctl(identificadorDaFila, IPC_RMID, NULL) == -1)
	{
		fprintf(stderr, "Erro ao remover fila de mensagem\n");
		exit(0);
	}
}

