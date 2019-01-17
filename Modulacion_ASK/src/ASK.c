#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"

#define PI 3.141592653589793

char analisis(short *segmento1, short *segmento2, short valorMaximo)
{
	unsigned char a = 0x00;
	unsigned char aux;
	valorMaximo -= 5;
	for(int i = 1, j = 0; i < 16; i = i + 2, j++)
	{
		if((segmento1[i] > (valorMaximo) && segmento2[i] == 0 ) | 
		   (segmento1[i] > (valorMaximo) && segmento2[i] > (valorMaximo)) | 
		   (segmento2[i] > (valorMaximo) && segmento1[i] == 0 ))

		{
			aux = pow(2, 7 - j);
			a = a | aux;
		}
	}

	printf("%c", a);
	return a;
}


void DFT(short *muestrasAudio, int numMuestrasAudio)
{

	short  *muestrasAudio2S = malloc(sizeof(short)  * numMuestrasAudio);
	short  *muestrasAudio1S = malloc(sizeof(short)  * numMuestrasAudio);
	double *muestrasAudio1  = malloc(sizeof(double) * numMuestrasAudio);
	double *muestrasAudio2  = malloc(sizeof(double) * numMuestrasAudio);

	for(int k = 0; k < numMuestrasAudio; k++)
	{
		muestrasAudio1[0] = 0;
		muestrasAudio2[0] = 0;
		for(int n = 0; n < numMuestrasAudio; n++)	
			muestrasAudio1[k] += muestrasAudio[n] * cos(2 * PI * k * n / numMuestrasAudio);
	
		muestrasAudio1[k]/=numMuestrasAudio;
		for(int n=0;n<numMuestrasAudio;n++)
			muestrasAudio2[k]+=muestrasAudio[n]*sin(2*PI*k*n/numMuestrasAudio);
				
		muestrasAudio2[k]/=numMuestrasAudio;
		muestrasAudio2[k]*=-1;
	}

	for(int i = 0; i < numMuestrasAudio; i++)
	{
		muestrasAudio2S[i] = (short)muestrasAudio2[i];
		muestrasAudio1S[i] = (short)muestrasAudio1[i];
	}

	analisis(muestrasAudio1S, muestrasAudio2S, 2000);
}

int main(int argc, char *argv[]){

	if(argc < 2)
	{
		printf("Ejecute con parametros: [nombre archivo wav]");
	}

	FILE *archivoWavEntrada = fopen(argv[1], "r");
	if(archivoWavEntrada == NULL)
	{
		printf("Error en la lectura del archivo de audio\n");
		exit(1);
	}
	else
	{
		Cabecera CW;
		fread(&CW, 44, 1, archivoWavEntrada);
		short *segmento       = malloc(sizeof(short) * 32);
		int numMuestrasAudio  = getNumberAudioSamples(CW);
		unsigned int numTotal = numMuestrasAudio / 32;
		//DFT
		for(char i = 0; i < numTotal; i++){
			fread(segmento, 64, 1, archivoWavEntrada);
			DFT(segmento, 32);
		}
			
		fclose(archivoWavEntrada);
	}
	return 0;
}
