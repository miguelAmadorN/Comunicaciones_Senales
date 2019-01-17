#include <stdlib.h>
#include <stdio.h>
#include "wav.h"


void modificacionVolumenAudio8bits(double factorDeModificacion, unsigned int numeroMuestras, 
 				   				   FILE *archivoWavEntrada,FILE *archivoWavSalida)
{
	unsigned char muestra;
	for(unsigned int i = 0; i < numeroMuestras; i++)
	{
		fread(&muestra, 1, 1, archivoWavEntrada);
		muestra = muestra * factorDeModificacion;
		fwrite(&muestra, 1, 1, archivoWavSalida);
	}
}

void modificacionVolumenAudio16bits(double factorDeModificacion, unsigned int numeroMuestras,
	                                FILE *archivoWavEntrada, FILE *archivoWavSalida)
{
	short muestra;
	for(unsigned int i = 0; i < numeroMuestras; i++)
	{
		fread(&muestra, 2, 1, archivoWavEntrada);
		muestra = muestra * factorDeModificacion;
		fwrite(&muestra, 2, 1, archivoWavSalida);
	}
}

void modificacionVolumenAudio24bits(double factorDeModificacion, unsigned int numeroMuestras,
	                                FILE *archivoWavEntrada, FILE *archivoWavSalida)
{
	int muestra = 0;
	for(unsigned int i = 0; i < numeroMuestras; i++)
	{
		fread(&muestra, 3 * sizeof(char), 1, archivoWavEntrada);
		muestra = muestra * factorDeModificacion;
		fwrite(&muestra, 3 * sizeof(char), 1, archivoWavSalida);
	}
}

void modificacionVolumenAudio32bits(double factorDeModificacion, unsigned int numeroMuestras,
									FILE *archivoWavEntrada, FILE *archivoWavSalida)
{
	int muestra;
	for(unsigned int i = 0; i < numeroMuestras; i++)
	{
			fread(&muestra, 4, 1, archivoWavEntrada);
			muestra = muestra * factorDeModificacion;
			fwrite(&muestra, 4, 1, archivoWavSalida);
	}
}


int main(int argc, char *argv[]){

	if(argc < 3)
	{
		printf("Ejecutar con parametros: [wav entrada] [wav salida] \n");
		exit(0);
	}

	Cabecera CW;
	FILE *archivoWavEntrada, *archivoWavSalida;
	archivoWavEntrada = fopen(argv[1], "r");

	if(archivoWavEntrada == NULL)
	{
		printf("Error en la lectura del archivo de audio\n");
	}
	else
	{
		archivoWavSalida = fopen(argv[2], "w");
		fread(&CW, 44, 1, archivoWavEntrada);
		fwrite(&CW, 44, 1, archivoWavSalida);
		int tamEnBytesMuestra = getSampleSize(CW);
		printf("Sample %d\n", tamEnBytesMuestra);
		unsigned int numeroMuestras = getTotalNumberSamples(CW);
/**
*	Recordar que si el archivo wav es estereo la muestra tendrá el doble de bytes pero se tendran que trabajar 
*	por separado. Por lo cual no afecta.
**/

		informacionAudio(CW);

		if(tamEnBytesMuestra == 1)
		{
			modificacionVolumenAudio8bits(.5, numeroMuestras, archivoWavEntrada, 
				                          archivoWavSalida);
		}else if(tamEnBytesMuestra == 2)
		{
			modificacionVolumenAudio16bits(.5, numeroMuestras, archivoWavEntrada, 
				                           archivoWavSalida);
		}else if(tamEnBytesMuestra == 3)
		{
			modificacionVolumenAudio24bits(.5, numeroMuestras, archivoWavEntrada, 
				                           archivoWavSalida);
		}else if(tamEnBytesMuestra == 4)
		{
			modificacionVolumenAudio32bits(.5, numeroMuestras, archivoWavEntrada, 
				                           archivoWavSalida);
		}

		unsigned int bytesSobrantes = getNumberBytesFoot(CW);

		/**
		*	Terminamos de copiar la informacion restante no sabemos que sea esa informacion pero 
		*	si sabemos que debe conicidir con  chunkSize = subchunk2_Size - 36 o lo que es igual
		* 	la cabecera de 44bytes con toda la demás información es igual al chunkSize  
		**/

		char muestrasSobrantes[bytesSobrantes];
		fread(muestrasSobrantes, bytesSobrantes, 1, archivoWavEntrada);
		fwrite(muestrasSobrantes, bytesSobrantes, 1, archivoWavSalida);
		fclose(archivoWavEntrada);
		fclose(archivoWavSalida);
	}
	return 0;

}




