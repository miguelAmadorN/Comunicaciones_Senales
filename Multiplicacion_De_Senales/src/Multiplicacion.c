#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"

#define PI 3.141592653589793

char getTipoMultiplicacion(char wav1, char wav2)
{
	if(wav1==1){
		if(wav2==1)
			return 1;
		else
			return 2;
	}else{
		if(wav2==1)
			return 2;
		else
			return 3;
	}

} 

double absDouble(double numero){
	if(numero<0)
		return numero*(-1);
	return numero;
}

int absNum(int numero){
	if(numero<0)
		return numero*(-1);
	return numero;
}

//muestras 1 es el arreglo mas grande
double* multiplicacionStereo(short *muestras1, short *muestras2, unsigned int numMuestras1, unsigned int tope)
{
	double *mult = malloc(sizeof(double) * numMuestras1);
	for(unsigned int i = 0; i < tope; i++)
	{
		mult[i] = (double)(muestras1[i] * muestras2[i]);
		mult[i] = mult[i] / 32767;
		if(mult[i] > 32767) mult[i] = 32767;
	}
		
	for(unsigned int i = tope; i < numMuestras1; i++)
		mult[i] = 0;	
	
	return mult;
} 

short* multiplicacion(short *muestras1, short *muestras2, unsigned int numMuestras1, unsigned int tope)
{
	double *mult = malloc(sizeof(double) * numMuestras1);	
	for(unsigned int i = 0; i < tope; i++)
	{
		mult[i]=(double)(muestras1[i] * muestras2[i]);
		mult[i]=mult[i] / 32767;
		if(mult[i] > 32767) mult[i] = 32767;
		muestras1[i] = mult[i];
	}
	for(unsigned int i = tope; i < numMuestras1; i++)
		mult[i]=0;	
	
	return muestras1;
} 

int main(int argc, char *argv[])
{
	if(argc < 4)
	{
		printf("Ejecuta con parametros [nombre wav 1] [nombre wav 2] [nombre wav salida]");
		exit(0);
	}

	FILE *archivoWavEntrada1, *archivoWavEntrada2, *archivoWavSalida;
	archivoWavEntrada1 = fopen(argv[1], "r");
	archivoWavEntrada2 = fopen(argv[2], "r");

	if(archivoWavEntrada1 == NULL || archivoWavEntrada2 == NULL)
	{
		printf("Error en la lectura de los archivos de audio\n");
	}else{
		char *archivo1 = argv[1];
		char *archivo2 = argv[2];
		Cabecera CW1   = getHeader(archivo1);
		Cabecera CW2   = getHeader(archivo2);
		char tipoWav1  = getCannalNumber(CW1);
		char tipoWav2  = getCannalNumber(CW2);
		char tipoMult  = getTipoMultiplicacion(tipoWav1, tipoWav2);

		if(tipoMult == 2)
		{//archivo 1 siempre debe ser el monoaural
			if(getCannalNumber(CW2) == 1)
			{
				char *aux = archivo1;
				archivo1  = archivo2;
				archivo2  = aux;
			}
		}		
		CW1              = getHeader(archivo1);
		CW2              = getHeader(archivo2);
		archivoWavSalida = fopen(argv[3],"w");

		if(archivoWavSalida != NULL)
		{
			int numMuestrasAudio1 = getTotalNumberSamples(CW1);
			int numMuestrasAudio2 = getTotalNumberSamples(CW2);
			short *muestras1      = getAudioSamples16bits(archivo1);
			short *muestras2      = getAudioSamples16bits(archivo2);
				
			switch(tipoMult)
			{
				case 1:// Mono * Mono
					if(numMuestrasAudio1 <= numMuestrasAudio2)
					{
						fwrite(&CW2, 44, 1, archivoWavSalida);
						fwrite(multiplicacion
							(muestras2, muestras1, numMuestrasAudio2, numMuestrasAudio1), 
							getNumberBytesAudioInformation(CW2), 1, archivoWavSalida);
							
							char *pie = getFileFoot(archivo2);
							fwrite(pie, getNumberBytesFoot(CW2), 1, archivoWavSalida);
						}
						else
						{
							fwrite(&CW1, 44, 1, archivoWavSalida);
							fwrite(multiplicacion
								(muestras1,muestras2,numMuestrasAudio1,numMuestrasAudio2),
								getNumberBytesAudioInformation(CW1),1,archivoWavSalida);
							char *pie = getFileFoot(archivo1);
							fwrite(pie, getNumberBytesFoot(CW1), 1, archivoWavSalida);
						}

						break;
					case 2://Mono * Stereo
						if(numMuestrasAudio1 < (numMuestrasAudio2 / 2))
						{
							fwrite(&CW2, 44, 1, archivoWavSalida);
							numMuestrasAudio2 /= 2;
							short *canal1 = multiplicacion(getAudioSamples16bisCanal1
									(archivo2), muestras1, numMuestrasAudio2,
									numMuestrasAudio1);
							short *canal2 = multiplicacion(getAudioSamples16bisCanal2
									(archivo2), muestras1, numMuestrasAudio2,
									numMuestrasAudio1);

							fwrite(getSalidaStereo(canal1, canal2, numMuestrasAudio2), 
								getNumberBytesAudioInformation(CW2),1,archivoWavSalida);

							char *pie=getFileFoot(archivo1);
								fwrite(pie,getNumberBytesFoot(CW2),1,archivoWavSalida);
						}
						else
						{
							CW1 = setAudioStereo(CW1);
							fwrite(&CW1, 44, 1, archivoWavSalida);
							numMuestrasAudio2 /= 2;
							short *canal1 = multiplicacion(muestras1, getAudioSamples16bisCanal1
									(archivo2),numMuestrasAudio1,numMuestrasAudio2);
							short *canal2 = multiplicacion(getAudioSamples16bits(archivo1),
									getAudioSamples16bisCanal2
                                                                        (archivo2),numMuestrasAudio1,numMuestrasAudio2);

							fwrite(getSalidaStereo(canal1, canal2, numMuestrasAudio1),
							       getNumberBytesAudioInformation(CW1),1,archivoWavSalida);

							char *pie=getFileFoot(archivo2);
							fwrite(pie,getNumberBytesFoot(CW2),1,archivoWavSalida);
						}
						break;
					case 3:// Stereo * Stereo
						if(numMuestrasAudio1 <= numMuestrasAudio2)
						{
							fwrite(&CW2, 44, 1, archivoWavSalida);
							numMuestrasAudio2 /= 2;
							numMuestrasAudio1 /= 2;
							double *op1 = multiplicacionStereo(getAudioSamples16bisCanal1(archivo2), 
								      getAudioSamples16bisCanal1(archivo1), numMuestrasAudio2,
								      numMuestrasAudio1);

							double *op2 = multiplicacionStereo(getAudioSamples16bisCanal2(archivo2),
								      getAudioSamples16bisCanal2(archivo1), numMuestrasAudio2,
								      numMuestrasAudio1);

							double *op3 = multiplicacionStereo(getAudioSamples16bisCanal2(archivo2),
								      getAudioSamples16bisCanal1(archivo1), numMuestrasAudio2,
								      numMuestrasAudio1);

							double *op4 = multiplicacionStereo(getAudioSamples16bisCanal1(archivo2),
								      getAudioSamples16bisCanal2(archivo1),numMuestrasAudio2,
								      numMuestrasAudio1);

							double *c1     = malloc(sizeof(double) * numMuestrasAudio2);
							double *c2     = malloc(sizeof(double) * numMuestrasAudio2);
							short  *canal1 = malloc(sizeof(short)  * numMuestrasAudio2);
							short  *canal2 = malloc(sizeof(short)  * numMuestrasAudio2);
							
							for(unsigned int i=0;i<numMuestrasAudio2;i++){
								c1[i] = (op1[i] - op2[i]) / 2;//Sin perdida de infomación
								c2[i] = (op3[i] + op4[i]) / 2;//
							/*	
								//Con perdida de informacion
								c1[i] = (op1[i] - op2[i]);
								c2[i] = (op3[i] + op4[i]);
								if(c1[i] >  32767) c1[i] =  32767;
								if(c2[i] >  32767) c2[i] =  32767;
								if(c1[i] < -32768) c1[i] = -32768;
								if(c2[i] < -32768) c2[i] = -32768;
							*/
								canal1[i] = c1[i];
								canal2[i] = c2[i];
							}

							fwrite(getSalidaStereo(canal1, canal2, numMuestrasAudio2),
								getNumberBytesAudioInformation(CW2), 1, archivoWavSalida);
							char *pie = getFileFoot(archivo2);
							fwrite(pie, getNumberBytesFoot(CW2), 1, archivoWavSalida);
						}
						else
						{
							fwrite(&CW1, 44, 1, archivoWavSalida);
							numMuestrasAudio2 /= 2;
							numMuestrasAudio1 /= 2;
							double *op1 = multiplicacionStereo(getAudioSamples16bisCanal1(archivo1),
								      getAudioSamples16bisCanal1(archivo2),numMuestrasAudio1,
								      numMuestrasAudio2);

							double *op2 = multiplicacionStereo(getAudioSamples16bisCanal2(archivo1),
								      getAudioSamples16bisCanal2(archivo2), numMuestrasAudio1,
								      numMuestrasAudio2);

							double *op3 = multiplicacionStereo(getAudioSamples16bisCanal1(archivo1),
								      getAudioSamples16bisCanal2(archivo2), numMuestrasAudio1,
								      numMuestrasAudio2);

							double *op4 = multiplicacionStereo(getAudioSamples16bisCanal2(archivo1),
								      getAudioSamples16bisCanal1(archivo2), numMuestrasAudio1,
								      numMuestrasAudio2);

							double *c1     = malloc(sizeof(double) * numMuestrasAudio1);
							double *c2     = malloc(sizeof(double) * numMuestrasAudio1);
							short  *canal1 = malloc(sizeof(short)  * numMuestrasAudio1);
							short  *canal2 = malloc(sizeof(short)  * numMuestrasAudio1);
	
							for(unsigned int i = 0; i < numMuestrasAudio2; i++)
							{
								c1[i] = (op1[i] - op2[i]) / 2;//Sin perdida
								c2[i] = (op3[i] + op4[i]) / 2;//
							/*
								//con perdida
								c1[i] = (op1[i] - op2[i]);
								c2[i] = (op3[i] + op4[i]);
								if(c1[i] >  32767) c1[i] =  32767;
								if(c2[i] >  32767) c2[i] =  32767;
								if(c1[i] < -32768) c1[i] = -32768;
								if(c2[i] < -32768) c2[i] = -32768;
							*/
								canal1[i]=c1[i];
								canal2[i]=c2[i];
							}

							fwrite(getSalidaStereo(canal1, canal2, numMuestrasAudio1),
							      getNumberBytesAudioInformation(CW1), 1, archivoWavSalida);

							char *pie = getFileFoot(archivo1);
							fwrite(pie, getNumberBytesFoot(CW1), 1, archivoWavSalida);
						}
						break;
					default:
						printf("Error\n");
				}

				fclose(archivoWavSalida);
				fclose(archivoWavEntrada1);
				fclose(archivoWavEntrada2);
					return 0;
			}
			else
			{
				printf("No se pudo crear el archivo de salida\n");
				return 1;
			}	
	}
	return 0;

}

