#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"
#define PI 3.141592653589793

typedef struct NumeroComplejo
{	
	double real;
	double imaginario;
}complejo;

int main(int argc, char *argv[])
{

	if(argc < 3)
	{
		printf("Ejecute con parametros: [nombre wav entrada] [nombre wav salida]\n");
		exit(0);
	}

	FILE *archivoWavEntrada1 = fopen(argv[1], "r");

	if(archivoWavEntrada1 == NULL)
	{
		printf("Error en la lectura del archivo de audio\n");
	}
	else
	{
		Cabecera CW1;
		fread(&CW1, 44, 1, archivoWavEntrada1);
		if(getCannalNumber(CW1) == 1)
		{
			FILE *archivoWavSalida = fopen(argv[2], "w");

			if(archivoWavSalida != NULL)
			{
				unsigned int numMuestrasAudio1 = getNumberAudioSamples(CW1);
				unsigned int bytesPie          = getNumberBytesFoot(CW1);
				short *x;
				float log                      = log2(numMuestrasAudio1);
				unsigned short logi            = log2(numMuestrasAudio1);
				char portenciaDeDos            = 0;
				unsigned int numMuestrasAntes  = numMuestrasAudio1;

				if(log - logi != 0)
				{
					logi++;
					numMuestrasAudio1 = pow(2, logi);
					CW1               = addNumberAudioSamples(CW1, numMuestrasAudio1 - numMuestrasAntes);
					portenciaDeDos    = 1;
				}
				
				unsigned int bytesAudio = getNumberBytesAudioInformation(CW1);
				CW1 = setAudioStereo(CW1);
				fwrite(&CW1, 44, 1, archivoWavSalida);
				
				if(portenciaDeDos)
				{
					x = malloc(numMuestrasAudio1 * 2 * sizeof(short));
					fread(x, numMuestrasAntes * 2, 1, archivoWavEntrada1);
					for(unsigned int i = numMuestrasAntes;i < numMuestrasAudio1; i++) x[i]=0;
				}
				else
				{
					x = malloc(sizeof(short) * numMuestrasAudio1 * 2);
					fread(x,numMuestrasAudio1*2,1,archivoWavEntrada1);
				}
				unsigned int bytesSobrantes = CW1.ChunkSize - CW1.Subchunk2_Size - 36;
				char *pie = malloc(sizeof(char) * bytesSobrantes);
				fread(pie, bytesSobrantes, 1, archivoWavEntrada1);
	
				//L=filas M=columnas
				unsigned char L = 32;
				unsigned int M  = numMuestrasAudio1 / L;
				complejo **F    = (complejo **)malloc(sizeof(complejo*) * L);
				complejo **X    = (complejo **)malloc(sizeof(complejo*) * L);
				
				for(int i = 0; i < L; i++)
				{
					F[i] = malloc(sizeof(complejo) * M);
					X[i] = malloc(sizeof(complejo) * M);				
				} 

				double C_W, C_W2;
				double cons = 2 * PI / M;
				double cons2= 2 * PI / numMuestrasAudio1;
				double r1, i1, r2, i2;
				
				for(unsigned int l = 0; l < L; l++){
					C_W2 = cons2 * l;
					for(unsigned int q = 0; q < M; q++)
					{
						C_W                = cons * q;
						F[l][q].real       = 0;
						F[l][q].imaginario = 0;

						for(unsigned int m = 0; m < M; m++)
						{
							F[l][q].real       += x[(L * m) + l] * cos(C_W * m);
							F[l][q].imaginario += x[(L * m) + l] * sin(C_W * m);

						}
						F[l][q].imaginario *= -1; 
						r1                  = F[l][q].real;
						i1                  = F[l][q].imaginario;
						r2                  = cos(C_W2 * q);
						i2                  = sin(C_W2 * q);
						F[l][q].real        = (r1 * r2 + i1 * i2);
						F[l][q].imaginario  = (i1 * r2 - r1 * i2);
					}
				}
			
				cons = 2 * PI / L;

				short *muestras1 = malloc(sizeof(short) * numMuestrasAudio1 * 2);
				int indice;
				for(unsigned int p = 0; p < L; p++)
				{
					for(unsigned int q = 0; q < M; q++)
					{
						C_W                = cons * p;
						X[p][q].real       = 0;
						X[p][q].imaginario = 0;

						for(unsigned int l = 0; l < L; l++)
						{
							r1 = F[l][q].real;
							i1 = F[l][q].imaginario;
							r2 = cos(C_W*l);
							i2 = sin(C_W*l);
							X[p][q].real       += (r1 * r2 + i1 * i2);
							X[p][q].imaginario += (i1 * r2 - r1 * i2);
						}
						indice              = 2 * (p * M + q);
						muestras1[indice]   = (short)(X[p][q].real / (numMuestrasAudio1));
						muestras1[indice+1] = (short)(X[p][q].imaginario / (numMuestrasAudio1));
					}
				}

				fwrite(muestras1, bytesAudio * 2, 1, archivoWavSalida);
				fwrite(pie, bytesPie, 1, archivoWavSalida);
				fclose(archivoWavSalida);
				fclose(archivoWavEntrada1);
			}
			else
			{
				printf("No se pudo crear el archivo de salida\n");
			}
		}
		else
		{
			printf("Archivo Stereo\n");
		}

	}	
	
}




