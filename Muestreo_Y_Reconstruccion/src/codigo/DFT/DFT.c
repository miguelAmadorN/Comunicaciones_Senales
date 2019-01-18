#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wav.h"

#define PI 3.141592653589793

double absDouble(double numero) 
{
    if (numero < 0)
        return numero * (-1);
    return numero;
}

int absNum(int numero) 
{
    if (numero < 0)
        return numero * (-1);
    return numero;
}

int main(int argc, char *argv[]) 
{
    FILE *archivoWavEntrada = fopen(argv[1], "r");

    if (archivoWavEntrada == NULL) {
        printf("Error en la lectura del archivo de audio\n");
    } else {

        FILE *archivoWavSalida = fopen(argv[2], "w");

        if (archivoWavSalida != NULL) 
	{
            Cabecera CW = getHeader(argv[1]);
            CW = setAudioStereo(CW);
            fwrite(&CW, 44, 1, archivoWavSalida);
            short *muestrasAudio   = getAudioSamples16bits(argv[1]);
            CW                     = getHeader(argv[1]);
            int numMuestrasAudio   = getNumberAudioSamples(CW);
            short *muestrasAudio2S = malloc(sizeof(short)  * numMuestrasAudio * 2);
            double *muestrasAudio2 = malloc(sizeof(double) * numMuestrasAudio * 2);
            //DFT

            int valorMaximoEntrada = absNum(muestrasAudio[0]);
            for (int i = 1; i < numMuestrasAudio; i++) 
	    {
                if (valorMaximoEntrada < absNum(muestrasAudio[i])) 
		{
                    valorMaximoEntrada = absNum(muestrasAudio[i]);
                }
            }

            for (int k = 0; k < numMuestrasAudio; k++) 
	    {
                muestrasAudio2[2 * k]     = 0;
                muestrasAudio2[2 * k + 1] = 0;
                for (int n = 0; n < numMuestrasAudio; n++) 
		{
                    muestrasAudio2[2 * k] += muestrasAudio[n] * cos(2 * PI * k * n / numMuestrasAudio);
                }
                for (int n = 0; n < numMuestrasAudio; n++) 
		{
                    muestrasAudio2[2 * k + 1] += muestrasAudio[n] * sin(2 * PI * k * n / numMuestrasAudio);
                }
                muestrasAudio2[2 * k + 1] *= -1;
            }

            double valorMaximo = absDouble(muestrasAudio2[0]);
            for (int i = 1; i < numMuestrasAudio * 2; i++) 
	    {
                if (valorMaximo < absDouble(muestrasAudio2[i]))
                    valorMaximo = absDouble(muestrasAudio2[i]);
            }

            for (int i = 0; i < numMuestrasAudio * 2; i++) 
	    {
                muestrasAudio2[i] = muestrasAudio2[i] * valorMaximoEntrada / valorMaximo;
                if (muestrasAudio2[i] > 32767)
                    muestrasAudio2[i] = 32767;
                if (muestrasAudio2[i] < -32768)
                    muestrasAudio2[i] = -32768;
                muestrasAudio2S[i] = muestrasAudio2[i];
            }

            unsigned int bytesAudio = getNumberBytesAudioInformation(CW);
            fwrite(muestrasAudio2S, bytesAudio * 2, 1, archivoWavSalida);

            char *pie = getFileFoot(argv[1]);
            fwrite(pie, getNumberBytesFoot(CW), 1, archivoWavSalida);
            fclose(archivoWavSalida);
            fclose(archivoWavEntrada);
        }
    }
}
