#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"
#define PI 3.141592653589793

int main(int argc, char *argv[]){

	if(argc < 3)
	{
		printf("Ejecute con parametros: [nombre wav entrada] [nombre wav salida]");
		exit(0);
	}
	FILE *archivoWavEntrada;
	archivoWavEntrada=fopen(argv[1], "r");

	if(archivoWavEntrada==NULL)
	{
		printf("Error en la lectura del archivo de audio\n");
	}
	else
	{
		Cabecera CW=getHeader(argv[1]);

		if(getCannalNumber(CW) == 2)
		{
			FILE *archivoWavSalida=fopen(argv[2], "w");

			if(archivoWavSalida != NULL)
			{
			
				fwrite(&CW, 44, 1, archivoWavSalida);
				short  *muestrasAudioCanal1 = getAudioSamples16bisCanal1(argv[1]);
				short  *muestrasAudioCanal2 = getAudioSamples16bisCanal2(argv[1]);
				int    numMuestrasAudio     = getNumberAudioSamples(CW);//Canal1 y canal 2
				double *muestrasAudioSalida = malloc(sizeof(double) * numMuestrasAudio * 2);
				short  *muestrasAudioFinal  = malloc(sizeof(short)  * numMuestrasAudio * 2);//incluira ambos canales

   
				double C = 2 * PI / numMuestrasAudio;
				for(int n = 0; n < numMuestrasAudio; n++)
				{
					muestrasAudioSalida[2 * n]     = 0;
					muestrasAudioSalida[2 * n + 1] = 0;
					for(int k = 0; k < numMuestrasAudio; k++)	//Parte Real
						muestrasAudioSalida[ 2 * n ]     += (( muestrasAudioCanal1[k] * cos(C * k * n))
										    -( muestrasAudioCanal2[k] * sin(C * k * n)));
					
					for(int k = 0; k < numMuestrasAudio; k++)	//Parte Imaginaria
						muestrasAudioSalida[ 2 * n + 1 ] += (( muestrasAudioCanal1[k] * sin(C * k * n))
                                                                                    +( muestrasAudioCanal2[k] * cos(C * k * n)));
				}


				numMuestrasAudio *= 2;
				for(int i = 0; i < numMuestrasAudio; i++)
					muestrasAudioFinal[i] = muestrasAudioSalida[i];
				
				unsigned int bytesAudio = getNumberBytesAudioInformation(CW);

				fwrite(muestrasAudioFinal, bytesAudio, 1, archivoWavSalida);
				char *pie = getFileFoot(argv[1]);
				fwrite(pie, getNumberBytesFoot(CW), 1, archivoWavSalida);
				fclose(archivoWavSalida);
				fclose(archivoWavEntrada);
				return 0;

			}
			else
			{
				printf("No se pudo crear el archivo de salida\n");
				return 0;
			}	
		}
		else
		{
			printf("El archivo es monoaural\n");
			return 0;
		}

	}
}
