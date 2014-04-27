#include <stdio.h>
#include <stdlib.h>	
#include <string.h>
int main (int argc, char* argv[])

{
	FILE* foto;

	foto = fopen(argv[1],"wb");
	fprintf(foto, "P5 %d %d 255\n",atoi(argv[2]),atoi(argv[3]) );

	fclose(foto);
}



