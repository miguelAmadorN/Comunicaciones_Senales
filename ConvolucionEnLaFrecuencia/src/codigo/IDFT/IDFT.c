
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

    FILE *archivoWavEntrada;
    archivoWavEntrada = fopen(argv[1], "r");

    if (archivoWavEntrada == NULL) 
    {
        printf("Error en la lectura del archivo de audio\n");

    } 
    else 
    {
        Cabecera CW = getHeader(argv[1]);

        if (getCannalNumber(CW) == 2) {
            FILE *archivoWavSalida = fopen(argv[2], "w");

            if (archivoWavSalida != NULL) 
	    {

                fwrite(&CW, 44, 1, archivoWavSalida);
                short *muestrasAudioCanal1 = getAudioSamples16bisCanal1(argv[1]);
                short *muestrasAudioCanal2 = getAudioSamples16bisCanal2(argv[1]);

                int numMuestrasAudio        = getNumberAudioSamples(CW);//Canal1 y canal 2
                double *muestrasAudioSalida = malloc(sizeof(double) * numMuestrasAudio * 2);
                short *muestrasAudioFinal   = malloc(sizeof(short)  * numMuestrasAudio * 2);//incluira ambos canales

                int valorMaximoEntradaCanal1 = absNum(muestrasAudioCanal1[0]);
                for (int i = 1; i < numMuestrasAudio; i++) 
		{
                    if (valorMaximoEntradaCanal1 < absNum(muestrasAudioCanal1[i]))
                        valorMaximoEntradaCanal1 = absNum(muestrasAudioCanal1[i]);
                }

                int valorMaximoEntradaCanal2 = absNum(muestrasAudioCanal2[0]);
                for (int i = 1; i < numMuestrasAudio; i++) 
		{
                    if (valorMaximoEntradaCanal2 < absNum(muestrasAudioCanal2[i]))
                        valorMaximoEntradaCanal2 = absNum(muestrasAudioCanal2[i]);
                }


                /**	Transforda Discreta de Fourier Inversa
                *	El canal 1 es la parte real, el canal 2 es la parte imaginaria.
                *
                **/

                double C = 2 * PI / numMuestrasAudio;

                for (int n = 0; n < numMuestrasAudio; n++) 
		{
                    muestrasAudioSalida[2 * n] = 0;
                    muestrasAudioSalida[2 * n + 1] = 0;
                    for (int k = 0; k < numMuestrasAudio; k++) {    //Parte Real
                        muestrasAudioSalida[2 * n] += ((muestrasAudioCanal1[k] * cos(C * k * n)) -
                                                       (muestrasAudioCanal2[k] * sin(C * k * n)));
                    }
                    muestrasAudioSalida[2 * n] /= numMuestrasAudio;

                    for (int k = 0; k < numMuestrasAudio; k++) {    //Parte Imaginaria
                        muestrasAudioSalida[2 * n + 1] += ((muestrasAudioCanal1[k] * sin(C * k * n)) +
                                                           (muestrasAudioCanal2[k] * cos(C * k * n)));
                    }
                    muestrasAudioSalida[2 * n + 1] /= numMuestrasAudio;
                }


                double valorMaximoC1 = absDouble(muestrasAudioSalida[0]);

                for (int i = 1; i < numMuestrasAudio * 2; i++) {
                    if (valorMaximoC1 < absDouble(muestrasAudioSalida[i]))
                        valorMaximoC1 = absDouble(muestrasAudioSalida[i]);
                }

                double constante;

                if (valorMaximoEntradaCanal1 < valorMaximoEntradaCanal2)
                    constante = valorMaximoEntradaCanal2 / valorMaximoC1;
                else
                    constante = valorMaximoEntradaCanal1 / valorMaximoC1;


                numMuestrasAudio *= 2;
                for (int i = 0; i < numMuestrasAudio; i++) {
                    muestrasAudioSalida[i] = muestrasAudioSalida[i] * constante;
                    if (muestrasAudioSalida[i] > 32767)  muestrasAudioSalida[i] =  32767;
                    if (muestrasAudioSalida[i] < -32768) muestrasAudioSalida[i] = -32768;
                    muestrasAudioFinal[i] = muestrasAudioSalida[i];
                }

                unsigned int bytesAudio = getNumberBytesAudioInformation(CW);

                fwrite(muestrasAudioFinal, bytesAudio, 1, archivoWavSalida);
                char *pie = getFileFoot(argv[1]);
                fwrite(pie, getNumberBytesFoot(CW), 1, archivoWavSalida);
                fclose(archivoWavSalida);
                fclose(archivoWavEntrada);

            } else {
                printf("No se pudo crear el archivo de salida\n");
            }
        } else {
            printf("El archivo el monoaural\n");
        }

    }
    return 0;
}
