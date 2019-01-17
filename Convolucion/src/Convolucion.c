#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"

#define PI 3.141592653589793

short Fc = 500;//500Hz
unsigned short Fs = 44100;//44100 muestras por segundo

double absDouble(double numero)
{
	if(numero < 0)
		return numero * ( -1 );
	return numero;
}

int absNum(int numero)
{
	if(numero < 0)
		return numero * ( -1 );
	return numero;
}


int main(int argc, char *argv[])
{

	if(argc < 4)
	{
		printf("Ejecutar con parametros [Nombre Wav entrada] [Nombre wav salida] [Num muestras]\n");
		exit(0);
	}

	FILE *archivoWavEntrada, *archivoWavSalida;
	archivoWavEntrada = fopen(argv[1], "r");
 
	if(archivoWavEntrada == NULL)
	{
		printf("Error en la lectura del archivo de audio\n");
	}
	else
	{
		double Constante = (2 * PI * Fc)/Fs;
		char numMuestras=100;
		double *h= malloc(sizeof(double)*numMuestras);

		/*
		*	Calculamos las muestras para el h(n)
		*/
		for(int i=0;i<numMuestras;i++)h[i] = exp(-i * Constante);


		archivoWavSalida=fopen(argv[2],"w");

		if(archivoWavSalida != NULL){
			Cabecera CW = getHeader(argv[1]);
			fwrite(&CW, 44, 1, archivoWavSalida);
			short *muestrasAudio          = getAudioSamples16bits(argv[1]);
			short *muestrasAudio2         = getAudioSamples16bits(argv[1]);
			double *muestrasAuxiliares    = malloc(sizeof(double) * numMuestras);
			unsigned int numMuestrasAudio = getNumberAudioSamples(CW);	

			int valorMaximoEntrada = absNum(muestrasAudio[0]);
			for(int i = 1; i < numMuestrasAudio; i++)
			{
				if(valorMaximoEntrada < absNum(muestrasAudio[i]))
					valorMaximoEntrada = absNum(muestrasAudio[i]);
			}
			if(valorMaximoEntrada > 32767)
				valorMaximoEntrada = 32767;

			//Convolucion
			double *muestrasAudio_2 = malloc(sizeof(double) * numMuestrasAudio);
			for(int i = 0; i < numMuestrasAudio; i++)
			{
				for(int j = 0; j < numMuestras; j++)
					muestrasAuxiliares[j] = h[j] * muestrasAudio[i];
		
				for(int p = 0; p < numMuestras && i < numMuestrasAudio; p++)
					muestrasAudio_2[i + p] += muestrasAuxiliares[p];
			}

			int valorMaximoSalida = absDouble(muestrasAudio_2[0]);
			for(int i = 1; i < numMuestrasAudio; i++)
			{
				if(valorMaximoSalida<absDouble(muestrasAudio_2[i]))
					valorMaximoSalida = absDouble(muestrasAudio_2[i]);
			}
			double factor =(double)valorMaximoEntrada / valorMaximoSalida;

			for(int i = 0; i < numMuestrasAudio; i++)
				muestrasAudio2[i] = muestrasAudio_2[i] * factor;
			
			
			unsigned int bytesAudio = getNumberBytesAudioInformation(CW);
			fwrite(muestrasAudio2, bytesAudio, 1, archivoWavSalida);

			char *pie = getFileFoot(argv[1]);
			fwrite(pie, getNumberBytesFoot(CW), 1, archivoWavSalida);
			fclose(archivoWavSalida);
			return 0;
		}
	}
}
