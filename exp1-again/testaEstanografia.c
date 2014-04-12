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


FILE *foto, *video, *chave;
unsigned char **imagem;
char h_imagem[7],w_imagem[7],h_video[7],w_video[7];
int i_img=0,j_img=0, num_bits;


pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;


void *desestenografar(){
	int i,j,k;
	
	unsigned char temp, temp1 =0,temp2=0,mask1 = 0x1,mask2=0x80;

	for(i=0; i<atoi(h_imagem); i++)	
		for(j=0; j<atoi(w_imagem); j++){
			printf("%x\t",imagem[i][j]);

			pthread_mutex_lock(&mut);
			temp=(unsigned char )imagem[i][j];			
			pthread_mutex_unlock(&mut);

			temp1 =0;
			temp2 = 0;
			for(k=0; k<=3; k++){
				pthread_mutex_lock(&mut);
				temp1|=(imagem[i][j]&(mask1<<k))<<(7-2*k);
				temp2|=(imagem[i][j]&(mask2>>k))>>(7-2*k);			
				pthread_mutex_unlock(&mut);
			}
			temp= temp1|temp2;
			temp = temp>>num_bits;
			temp = temp<<num_bits;
			
			pthread_mutex_lock(&mut);
			imagem[i][j]=temp;
			pthread_mutex_unlock(&mut);
			printf("%x\n",imagem[i][j]);
	
		}
	
	pthread_mutex_unlock(&mut);

}
void *desembaralha(){
	int i,j;
	for(i=0 ; i<atoi(h_imagem);i++)
		for(j=0 ; j<atoi(w_imagem);j++)		
				fread(&imagem[i][j],sizeof(unsigned char),1,video);
}
int main(){

	FILE *foto, *video, *chave;
	pthread_t thread_id,thread_1,thread_0;
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

	if(!(video=fopen("teste_EsteganografiaLSB_1280x720_1bit_.y","r+"))){
		printf("arquivo nao pode ser aberto");
		exit(0);
	}

	imagem = (unsigned char **) malloc (atoi(h_imagem)*sizeof(unsigned char*));
	for(i=0 ; i<atoi(h_imagem);i++)
		imagem[i] = (unsigned char *) malloc (atoi(w_imagem)*sizeof(unsigned char));

	pthread_create(&thread_0,NULL,desembaralha,NULL);
	pthread_join(thread_0,NULL);
	pthread_create(&thread_1,NULL,desestenografar,NULL);


	pthread_join(thread_1,NULL);


	for(i=0 ; i<atoi(h_imagem);i++)
		free(imagem[i]);
	free(imagem);
}
