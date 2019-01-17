#include "wav.h"
#include <stdio.h>
#include <stdlib.h>

void informacionAudio(Cabecera cabeceraAudio)
{
	printf("%c%c%c%c\n", cabeceraAudio.ChunkID[0], cabeceraAudio.ChunkID[1],
		   cabeceraAudio.ChunkID[2],cabeceraAudio.ChunkID[3]);

	printf("ChunkSize: %d\n",cabeceraAudio.ChunkSize);

	printf("%c%c%c%c\n", cabeceraAudio.Format[0], cabeceraAudio.Format[1],
		    cabeceraAudio.Format[2],cabeceraAudio.Format[3]);

	printf("%c%c%c%c\n", cabeceraAudio.Subchunk1_ID[0], cabeceraAudio.Subchunk1_ID[1],
		    cabeceraAudio.Subchunk1_ID[2],cabeceraAudio.Subchunk1_ID[3]);

	printf("Subchunk1_Size:%d   %d%d%d\n", cabeceraAudio.Subchunk1_Size[0], 
		    cabeceraAudio.Subchunk1_Size[1],cabeceraAudio.Subchunk1_Size[2],
		    cabeceraAudio.Subchunk1_Size[3]);
	
	printf("Formato del audio: %d\n", cabeceraAudio.AudioFormat[0] );

	printf("Numero de canales: %d\n",cabeceraAudio.NumChannels[0] );

	printf("Muestras por segundo %d\n", cabeceraAudio.SampleRate);

	printf("Bytes por segundo: %d\n", cabeceraAudio.ByteRate);

	printf("Numero de bytes por muestra %d\n", cabeceraAudio.BlockAlign );

	printf("Numero de bits por muestra: %d\n", cabeceraAudio.BitsPerSample);

	printf("%c%c%c%c\n", cabeceraAudio.Subchunk2_ID[0], cabeceraAudio.Subchunk2_ID[1],
		    cabeceraAudio.Subchunk2_ID[2],cabeceraAudio.Subchunk2_ID[3]);

	printf("Numero de bytes de la informacion %d\n",cabeceraAudio.Subchunk2_Size);

	printf("Numero de muestras %d\n", cabeceraAudio.Subchunk2_Size / (cabeceraAudio.NumChannels[0]
		    * (cabeceraAudio.BitsPerSample/8)));

	printf("Bytes de sobra (informaciondesconocida): %d\n", cabeceraAudio.ChunkSize - 36 
		    - cabeceraAudio.Subchunk2_Size);

	printf("Tam total del archivo: %d\n", cabeceraAudio.ChunkSize + 8);	
}

//Regresa el tamaño completo del archivo en bytes
unsigned int getFileSize(Cabecera CW)
{
	return CW.ChunkSize + 8;
}

//Regresa el tamaño de las muestras en bytes
char getSampleSize(Cabecera CW)
{
	return (CW.BitsPerSample / 8);	
}

//Regresa el numero de canales;
char getCannalNumber(Cabecera CW)
{
	return CW.NumChannels[0];
}

//Regresa el numero de muestras de audio si es momo o estero regresa la misma cantidad por lo cual en estereo se debe multiplicar por 2
unsigned int getNumberAudioSamples(Cabecera CW)
{
	return CW.Subchunk2_Size / (CW.NumChannels[0] * (CW.BitsPerSample / 8));
}

//Regresa el numero de muestras de audio totales
unsigned int getTotalNumberSamples(Cabecera CW)
{
	return CW.Subchunk2_Size / (CW.BitsPerSample / 8);
}

//Agrega una cantidad de muestras a la cabecera sin importar si es mono o estereo
Cabecera addNumberAudioSamples(Cabecera CW, int numberSamples)
{
	CW.Subchunk2_Size += (numberSamples * 2 * CW.NumChannels[0]);
	CW.ChunkSize      += (numberSamples*2*CW.NumChannels[0]);
	return CW;
}

//Regresa el tamanio total del archivo wav-8 
unsigned int getNumberBytesAudioInformation(Cabecera CW)
{
	return CW.Subchunk2_Size;
}

//Convierte una cabecera de tipo mono a una de tipo estereo
Cabecera setAudioStereo(Cabecera CW)
{
	CW.ChunkSize      += CW.Subchunk2_Size;
	CW.Subchunk2_Size *= 2;
	CW.NumChannels[0] = 2;
	CW.ByteRate       *= 2;
	CW.BlockAlign     *= 2;
	return CW;
}

//regresa el numero de bytes del pie del archivo wav
unsigned int getNumberBytesFoot(Cabecera CW)
{
	return CW.ChunkSize - 36 - CW.Subchunk2_Size;
}

//Regresa la cabecera del archvo wav
Cabecera getHeader(char *archivoWavEntrada)
{
	Cabecera CW;
	FILE *archivo = fopen(archivoWavEntrada, "r");
	fread(&CW, 44, 1, archivo);
	fclose(archivo);
	return CW;
}

//Regresa las muestras de un archivo con tamanio de 8 bits
char* getAudioSamples8bits(char *archivoWavEntrada)
{
	Cabecera CW;
	FILE *archivo = fopen(archivoWavEntrada, "r");
	fread(&CW, 44, 1, archivo);
	unsigned int numeroMuestras = CW.Subchunk2_Size / (CW.NumChannels[0] * (CW.BitsPerSample / 8));
	char *Samples = malloc(sizeof(char) * numeroMuestras);
	fread(Samples, numeroMuestras, 1, archivo);
	fclose(archivo);
	return Samples;
}	

//Regresa las muestras de audio de 16 bits mono/Stereo
short* getAudioSamples16bits(char *archivoWavEntrada)
{
	Cabecera CW;
	FILE *archivo = fopen(archivoWavEntrada,"r");
	fread(&CW, 44, 1, archivo);
	unsigned int numeroMuestras = CW.Subchunk2_Size / (CW.NumChannels[0] * (CW.BitsPerSample / 8));
	short *Samples = malloc(sizeof(short) * numeroMuestras * CW.NumChannels[0]);
	fread(Samples,numeroMuestras * 2 * CW.NumChannels[0], 1, archivo);
	fclose(archivo);
	return Samples;
}
short* getAudioSamples16bitsStereo(char *archivoWavEntrada){
	Cabecera CW;
	FILE *archivo = fopen(archivoWavEntrada, "r");
	fread(&CW, 44, 1, archivo);
	unsigned int numeroMuestras = CW.Subchunk2_Size / (CW.NumChannels[0] * (CW.BitsPerSample / 8));
	numeroMuestras *= 2;
	short *Samples = malloc(sizeof(short) * numeroMuestras);
	fread(Samples, numeroMuestras * 2, 1 , archivo);
	fclose(archivo);
	return Samples;
}

//Regresa las muestras de 16bits del canal1
short* getAudioSamples16bisCanal1(char *archivoWavEntrada){
	Cabecera CW = getHeader(archivoWavEntrada);

	if(getCannalNumber(CW) == 2)
	{
		short *muestrasAudio = getAudioSamples16bitsStereo(archivoWavEntrada);
		unsigned int tam     = getNumberAudioSamples(CW);
		short *canal1        = malloc(tam*sizeof(short));

		for(unsigned int i = 0 ; i < tam; i++)
			canal1[i] = muestrasAudio[2 * i];
		
		return canal1;
	}
	else
	{
		printf("El archivo: %s no es estereo\n", archivoWavEntrada);
		return NULL;
	}
}

//Regresa las muestras de 16bits del canal2
short* getAudioSamples16bisCanal2(char *archivoWavEntrada)
{
	Cabecera CW = getHeader(archivoWavEntrada);
	if(getCannalNumber(CW) == 2)
	{
		short *muestrasAudio = getAudioSamples16bitsStereo(archivoWavEntrada);
		unsigned int tam     = getNumberAudioSamples(CW);
		short *canal2        = malloc(tam * sizeof(short));

		for(unsigned int i = 0; i < tam; i++)
				canal2[i] = muestrasAudio[2 * i + 1];
		
		return canal2;
	}
	else
	{
		printf("El archivo: %s no es estereo\n", archivoWavEntrada);
		return NULL;
	}
}

//Regresa las bytes del pie del archivo wav
char* getFileFoot(char *archivoWavEntrada){
	Cabecera CW;
	FILE *archivo = fopen(archivoWavEntrada, "r");
	fread(&CW, 44, 1, archivo);
	unsigned int bytesSobrantes = CW.ChunkSize-CW.Subchunk2_Size-36;
	char *bytes = malloc(sizeof(char) * CW.Subchunk2_Size);
	fread(bytes, CW.Subchunk2_Size, 1, archivo);
	bytes = malloc(sizeof(char) * bytesSobrantes);
	fread(bytes, bytesSobrantes, 1, archivo);
	return bytes;
}

short* getSalidaStereo(short *canal1, short *canal2, unsigned int tamCanal)
{
	short *salida = malloc(sizeof(short) * tamCanal * 2);
	for(unsigned int i = 0; i < tamCanal; i++)
	{
		salida[2 * i]     = canal1[i];
		salida[2 * i + 1] = canal2[i];
	}
	return salida;
}
