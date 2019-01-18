#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"

#define PI 3.141592653589793

double absDouble(double numero)
{
	if(numero < 0)
		return numero * (-1);
	return numero;
}

short absShort(short numero)
{
	if(numero < 0)
		return numero * (-1);
	return numero;
}

void valor(char encontrados[]){
	if(encontrados[0])
		printf("SOL\n");
	if(encontrados[1])
		printf("SOL#\n");
	if(encontrados[2])
		printf("LA\n");
	if(encontrados[3])
		printf("LA#\n");
	if(encontrados[4])
		printf("SI\n");
	if(encontrados[5])
		printf("DO\n");
	if(encontrados[6])
		printf("DO#\n");
	if(encontrados[7])
		printf("RE\n");
	if(encontrados[8])
		printf("RE#\n");
	if(encontrados[9])
		printf("MI\n");
	if(encontrados[10])
		printf("FA\n");
	if(encontrados[11])
		printf("FA#\n");
	if(encontrados[12])
		printf("SOL\n");
	if(encontrados[13])
		printf("SOL#\n");
	if(encontrados[14])
		printf("LA\n");
}

int main(int argc, char *argv[])
{
	if(argc < 2)
	{
		printf("Ejecutar con parametros: [nombre archivo wav]");
		exit(0);
	}

	FILE *archivoWavEntrada=fopen(argv[1],"r");

	if(archivoWavEntrada==NULL)
	{
		printf("Error en la lectura del archivo de audio\n");
		exit(1),
	}
	else
	{
		Cabecera CW = getHeader(argv[1]);
		if(getCannalNumber(CW)==1)
		{

			short  *muestrasAudio   = getAudioSamples16bits(argv[1]);
			int    numMuestrasAudio = getNumberAudioSamples(CW);
			short  *muestrasAudio2S = malloc(sizeof(short)  * numMuestrasAudio * 2);
			double *muestrasAudio2  = malloc(sizeof(double) * numMuestrasAudio * 2);
				//DFT
			int valorMaximoEntrada= absShort(muestrasAudio[0]);
			for(int i = 1; i < numMuestrasAudio; i++)
			{
				if(valorMaximoEntrada < absShort(muestrasAudio[i]))
					valorMaximoEntrada = absShort(muestrasAudio[i]);
			}

			for(int k = 0; k < numMuestrasAudio; k++)
			{
				muestrasAudio2[2 * k]     = 0;
				muestrasAudio2[2 * k + 1] = 0;
				for(int n=0;n<numMuestrasAudio;n++)
				{	
					muestrasAudio2[2 * k] += muestrasAudio[n] * cos(2 * PI * k * n/ numMuestrasAudio);
				}
				for(int n=0;n<numMuestrasAudio;n++)
				{
					muestrasAudio2[2 * k + 1] += muestrasAudio[n] * sin(2 * PI * k * n / numMuestrasAudio);
				}
				muestrasAudio2[2*k+1]*=-1;
			}

			double valorMaximo = absDouble(muestrasAudio2[0]);
			for(int i = 1; i < numMuestrasAudio * 2; i++)
			{
				if(valorMaximo < absDouble(muestrasAudio2[i]))
					valorMaximo = absDouble(muestrasAudio2[i]);
			}

			for(int i = 0; i < numMuestrasAudio * 2; i++)
			{
				muestrasAudio2S[i] = muestrasAudio2[i] * valorMaximoEntrada / valorMaximo;
			}

			int rate          = getSampleRate(CW);
			double resolucion = (double)((double)rate / (double)numMuestrasAudio);
			char numFrec      = 15;
			short Frecuencias[] = { 392, 415, 440, 466, 494, 523, 554, 587, 622, 659, 698, 740, 784, 831, 880};
			unsigned int posiciones[numFrec];
			char encontrados[numFrec];
			for(unsigned int i = 0; i < numFrec; i++)
			{
				posiciones[i] = (unsigned int)(Frecuencias[i] / resolucion);	
			}
			
			for(unsigned int i=0;i<numFrec;i++){
				if(absShort(muestrasAudio2S[2 * posiciones[i]]) > 20000 ||
				   absShort(muestrasAudio2S[2 * posiciones[i] + 1]) > 20000 ||
				   absShort(muestrasAudio2S[2 * (posiciones[i] + 1)]) > 20000 ||
				   absShort(muestrasAudio2S[2 * (posiciones[i] + 1) + 1]) > 20000)
				{
					encontrados[i]=1;
				}
				else
				{
					encontrados[i]=0;
				}
			}
			valor(encontrados);				
		}
		else
		{
			printf("Archivo Estereo\n");
			exit(0);
		}
	}
	return 0;
}
