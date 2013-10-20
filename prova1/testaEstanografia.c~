#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <ctype.h>


unsigned char **imagem;
char h_imagem[7],w_imagem[7],h_video[7],w_video[7];
int i_img,j_img, num_bits;




void *desestenografar(){
	int i,j,k;

	for(i=0; i<atoi(h_imagem); i++)	
		for(j=0; j<atoi(w_imagem); j++){
			imagem[i][j]= imagem[i][j]<<8-num_bits;
			
}

int main(){
	FILE *foto, *video, *chave;
	pthread_t thread_id;
	int i,j;


	printf("Digite o h do imagem escondida->");
	scanf("%s",h_imagem);

	printf("Digite o h do imagem escondida->");
	scanf("%s",w_imagem);

	printf("Digite o h do video escondida->");
	scanf("%s",h_video);

	printf("Digite o h do video escondida->");
	scanf("%s",h_video);
	
	printf("Digite o numero de bits da esteganografia->");
	scanf("%d",&num_bits);

	imagem = (unsigned char **) malloc (atoi(h_imagem)*sizeof(unsigned char*));
	for(i=0 ; i<atoi(h_imagem);i++)
		imagem[i] = (unsigned char *) malloc (atoi(w_imagem)*sizeof(unsigned char));

	for(i=0 ; i<atoi(h_imagem);i++){
		for(j=0 ; j<atoi(w_imagem);j++)		
			printf("%d\t",imagem[i][j]);
		puts("\n");
	}



	for(i=0 ; i<atoi(h_imagem);i++)
		free(imagem[i]);
	free(imagem);
}
