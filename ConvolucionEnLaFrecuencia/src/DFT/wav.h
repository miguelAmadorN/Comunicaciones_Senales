#ifndef WAV_H
#define WAV_H
/**
*	La informacion de la cabecera se encuentra:
*	http://soundfile.sapp.org/doc/WaveFormat/ 
**/

typedef struct CabeceraWAV
{
	unsigned char ChunkID[4];
	unsigned int ChunkSize;
	unsigned char Format[4];
	unsigned char Subchunk1_ID[4];
	unsigned char Subchunk1_Size[4];
	unsigned char AudioFormat[2];
	unsigned char NumChannels[2];
	unsigned int SampleRate;
	unsigned int ByteRate;
	unsigned short BlockAlign;
	unsigned short BitsPerSample;
	unsigned char Subchunk2_ID[4];
	unsigned int Subchunk2_Size;
}Cabecera;
//Regresa el numero muestras por segundo
unsigned int getSampleRate(Cabecera CW);

//Imprime la informacion del audio
void informacionAudio(Cabecera cabeceraAudio);

//Resegra el tamaño completo del archivo en bytes
unsigned int getFileSize(Cabecera CW);

//Regresa el tamaño de las muestras en bytes
char getSampleSize(Cabecera CW);

//Regresa el numero de canales;
char getCannalNumber(Cabecera CW);

//Regresa el numero de muestras de audio si es momo o estero regresa la misma cantidad por lo cual en estereo se debe multiplicar por 2
unsigned int getNumberAudioSamples(Cabecera CW);

//Regresa el numero de muestras de audio totales
unsigned int getTotalNumberSamples(Cabecera CW);

//Agrega una cantidad de muestras a la cabecera sin importar si es mono o estereo
Cabecera addNumberAudioSamples(Cabecera CW, int numberSamples);

//Regresa el tamanio total del archivo wav-8 
unsigned int getNumberBytesAudioInformation(Cabecera CW);

//Convierte una cabecera de tipo mono a una de tipo estereo
Cabecera setAudioStereo(Cabecera CW);

//regresa el numero de bytes del pie del archivo wav
unsigned int getNumberBytesFoot(Cabecera CW);

//Regresa la cabecera del archvo wav
/**
*	archivoWavEntrada:nombre del archivo wav
**/
Cabecera getHeader(char *archivoWavEntrada);

//Regresa las muestras de un archivo con tamanio de 8 bits
/**
*	archivoWavEntrada:nombre del archivo wav
**/
char* getAudioSamples8bits(char *archivoWavEntrada);

//Regresa las muestras de audio de 16 bits mono/Stereo
/**
*	archivoWavEntrada:nombre del archivo wav
**/
short* getAudioSamples16bits(char *archivoWavEntrada);

short* getAudioSamples16bitsStereo(char *archivoWavEntrada);

//Regresa las muestras de 16bits del canal1
/**
*	archivoWavEntrada:nombre del archivo wav
**/
short* getAudioSamples16bisCanal1(char *archivoWavEntrada);

//Regresa las muestras de 16bits del canal2
/**
*	archivoWavEntrada:nombre del archivo wav
**/
short* getAudioSamples16bisCanal2(char *archivoWavEntrada);

//Regresa los bytes del pie del archivo wav
/**
*	archivoWavEntrada: nombre del archivo wav
**/
char* getFileFoot(char *archivoWavEntrada);

short* getSalidaStereo(short *canal1, short *canal2, unsigned int tamCanal);

#endif
