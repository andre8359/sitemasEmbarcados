#ifndef MESSAGE_H
#define MESSAGE_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define TAMANHO_TEXTO 4096

typedef struct my_msgbuf
{
	long mtype;
	char mtext[TAMANHO_TEXTO];
} buf;

extern int gerarIdentificadorDaFila(key_t gerarChaveParaFila);
extern int receberMensagemDaFila(int identificadorDaFila, struct my_msgbuf* buf);
extern void mandarMensagemParaFila(int identificadorDaFila, struct my_msgbuf* buf);
extern void retirarMensagemDaFila(int identificadorDaFila, struct my_msgbuf* buf);
extern void removerFilaDeMensagem(int identificadorDaFila);

#endif
