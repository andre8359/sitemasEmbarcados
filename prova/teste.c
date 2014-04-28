#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>



int main(int argc, char const *argv[])
{

	int pid;
	char *list_args[4][10] = {"md5sum","imagem_final.y",NULL},prog []="md5sum";

	pid = fork();


	if(!pid){
		execlp( "/bin/sh", "/bin/sh", "-c", "md5sum imagemfinal.y > hash.txt", (char *)NULL );
		printf("deu erradoo!\n");
		exit(1);	
	}
	
	printf("rezar pra dar !!!\n");
	

	return 0;
}