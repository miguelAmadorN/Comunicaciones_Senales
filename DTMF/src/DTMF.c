#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"

#define PI 3.141592653589793


double absDouble(double numero)
{
	if(numero < 0)
		return numero*(-1);
	return numero;
}

short absShort(short numero)
{
	if(numero < 0)
		return numero * (-1);
	return numero;
}

void valor(char encontrados[])
{
	if(encontrados[0]){
		if(encontrados[4])
			printf("1\n");
		if(encontrados[5])
			printf("2\n");
		if(encontrados[6])
			printf("3\n");
		if(encontrados[7])
			printf("A\n");
	}
	if(encontrados[1]){
		if(encontrados[4])
			printf("4\n");
		if(encontrados[5])
			printf("5\n");
		if(encontrados[6])
			printf("6\n");
		if(encontrados[7])
			printf("B\n");
	}
	if(encontrados[2]){
		if(encontrados[4])
			printf("7\n");
		if(encontrados[5])
			printf("8\n");
		if(encontrados[6])
			printf("9\n");
		if(encontrados[7])
			printf("C\n");
	}
	if(encontrados[3]){
		if(encontrados[4])
			printf("*\n");
		if(encontrados[5])
			printf("0\n");
		if(encontrados[6])
			printf("#\n");
		if(encontrados[7])
			printf("D\n");
	}
}

int main(int argc, char *argv[]){

	if(argc < 2)
	{
		printf("Ejecutar con parametros: [nombra archivo wav]");
		exit(0);
	}
	FILE *archivoWavEntrada=fopen(argv[1],"r");

	if(archivoWavEntrada==NULL)
	{
		printf("Error en la lectura del archivo de audio\n");
		exit(1);
	}
	else
	{
		Cabecera CW=getHeader(argv[1]);
		if(getCannalNumber(CW) == 1)
		{

			short *muestrasAudio   = getAudioSamples16bits(argv[1]);
			int numMuestrasAudio   = getNumberAudioSamples(CW);
			short *muestrasAudio2S = malloc(sizeof(short)  * numMuestrasAudio * 2);
			double *muestrasAudio2 = malloc(sizeof(double) * numMuestrasAudio * 2);
				//DFT
			for(int k=0;k<numMuestrasAudio;k++)
			{
				muestrasAudio2[2 * k]     = 0;
				muestrasAudio2[2 * k + 1] = 0;
				for(int n = 0; n < numMuestrasAudio; n++)	
					muestrasAudio2[ 2 * k ] += muestrasAudio[n] * cos(2 * PI * k * n / numMuestrasAudio);
				
				muestrasAudio2[ 2 * k ] /= numMuestrasAudio;
				for(int n = 0; n < numMuestrasAudio; n++)
					muestrasAudio2[2 * k + 1] += muestrasAudio[n] * sin(2 * PI * k * n / numMuestrasAudio);
					
				muestrasAudio2[ 2 * k + 1 ] /= numMuestrasAudio;
				muestrasAudio2[ 2 * k + 1 ] *= -1;
			}

			for(int i = 0; i < numMuestrasAudio * 2; i++)
				muestrasAudio2S[i] = muestrasAudio2[i];

			int rate          = getSampleRate(CW);
			double resolucion = (double)((double)rate / (double)numMuestrasAudio);
			short DTMF[]      = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
			unsigned int posiciones[8];
			char encontrados[8];
			for(short i = 0; i < 8; i++)
				posiciones[i] = (unsigned int)(DTMF[i] / resolucion);	
				
			for(short i = 0; i < 8; i++)
			{
				if(absShort(muestrasAudio2S[2 * posiciones[i]]) > 5000 || 
			           absShort(muestrasAudio2S[2 * posiciones[i] + 1]) > 5000 ||
				   absShort(muestrasAudio2S[2 * (posiciones[i] + 1)]) > 5000 || 
				   absShort(muestrasAudio2S[2 * (posiciones[i] + 1) + 1]) > 5000)
				{
					encontrados[i]=1;
				}
				else
				{
					encontrados[i]=0;
				}
			}
			valor(encontrados);				
		}else
		{
			printf("Archivo stereo\n");
			exit(0);
		}
	}
	return 0;
}
