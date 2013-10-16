#ifndef SEMAFORO_H
#define SEMAFORO_H

#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>

extern int criar_semaforo(key_t key, int numeroDeSemaforos);
extern int down(int identificadorSemaforo, int numeroDoSemaforo);
extern int up(int identificadorSemaforo, int numeroDoSemaforo);
extern int deletar_semaforo(int identificadorSemaforo, int numeroDoSemaforo);
extern int criarChave(int identificador);
extern int iniciarSemaforo(int semid, int valorInicial, int numeroDoSemaforo);
extern int getValorSemaforo(int identificadorDoSemaforo, int numeroDoSemaforo);

#endif
