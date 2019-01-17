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

int main(int argc, char *argv[]){

	FILE *archivoWavEntrada1=fopen(argv[1],"r");

	if(archivoWavEntrada1==NULL){
		printf("Error en la lectura del archivo de audio\n");
	}else{

		FILE *archivoWavSalida=fopen(argv[2],"w+");

		if(archivoWavSalida!=NULL){

				char *pie=getFileFoot(argv[1]);
				Cabecera CW1=getHeader(argv[1]);
				unsigned int numMuestrasAudio1=getNumberAudioSamples(CW1);
				unsigned int bytesPie=getNumberBytesFoot(CW1);

				short *canal1;
				short *canal2;
				
				float log=log2(numMuestrasAudio1);
				unsigned short logi=log2(numMuestrasAudio1);
				char portenciaDeDos=0;
				unsigned int numMuestrasAntes=numMuestrasAudio1;


				if(log-logi!=0){
					logi++;
					numMuestrasAudio1=pow(2,logi);
					CW1=addNumberAudioSamples(CW1,numMuestrasAudio1-numMuestrasAntes);
					portenciaDeDos=1;
				}

				canal1=getAudioSamples16bisCanal1(argv[1]);
				canal2=getAudioSamples16bisCanal2(argv[1]);
				if(canal1 == NULL || canal2 == NULL )
				{
					exit(0);				
				}

				if(portenciaDeDos){

					short *Acanal1=(short*)malloc(sizeof(short)*numMuestrasAudio1);
					short *Acanal2=(short*)malloc(sizeof(short)*numMuestrasAudio1);
					for(unsigned int i=0;i<numMuestrasAntes;i++){
						Acanal1[i]=canal1[i];
						Acanal2[i]=canal2[i];
					}

					for(unsigned int i=numMuestrasAntes;i<numMuestrasAudio1;i++){
						Acanal1[i]=0;
						Acanal2[i]=0;
					}

					canal1=(short*)malloc(sizeof(short)*numMuestrasAudio1);
					canal2=(short*)malloc(sizeof(short)*numMuestrasAudio1);
					canal1=Acanal1;
					canal2=Acanal2;
				}

				unsigned int bytesAudio=getNumberBytesAudioInformation(CW1);
				fwrite(&CW1,44,1,archivoWavSalida);


				//L=filas M=columnas
				unsigned char L=4;
				unsigned int M=numMuestrasAudio1/L;

				complejo **F=(complejo **)malloc(sizeof(complejo*)*L);
				complejo **x=(complejo **)malloc(sizeof(complejo*)*L);
				

				for(unsigned int i=0;i<L;i++){
					F[i]=malloc(sizeof(complejo)*M);
					x[i]=malloc(sizeof(complejo)*M);
				}

				double r1,i1,r2,i2;
				double C_W;
				double cons=(2*PI/M);
				for(unsigned int l=0;l<L;l++){
					for(unsigned int q=0;q<M;q++){
						C_W=cons*q;
						C_W=-C_W;
						F[l][q].real=0;
						F[l][q].imaginario=0;
						for(unsigned int m=0;m<M;m++){
								r2=cos(C_W*m);
								i2=sin(C_W*m);

								F[l][q].real+=canal1[(L*m)+l]*r2+canal2[(L*m)+l]*i2;
								F[l][q].imaginario+=canal2[(L*m)+l]*r2-canal1[(L*m)+l]*i2;
						}
					}
				}
				
				cons=2*PI/numMuestrasAudio1;
			
				for(unsigned int l=0;l<L;l++){
					C_W=cons*l;
					C_W=-C_W;
					for(unsigned int q=0;q<M;q++){
						r1=F[l][q].real;
						i1=F[l][q].imaginario;
						r2=cos(C_W*q);
						i2=sin(C_W*q);
						F[l][q].real=(r1*r2+i1*i2);
						F[l][q].imaginario=(i1*r2-r1*i2);
					}
				}

				cons=(2*PI/L);
				for(unsigned int p=0;p<L;p++){
					for(unsigned int q=0;q<M;q++){
						C_W=cons*p;
						C_W=-C_W;
						x[p][q].real=0;
						x[p][q].imaginario=0;
						for(unsigned int l=0;l<L;l++){
							r1=F[l][q].real;
							i1=F[l][q].imaginario;
							r2=cos(C_W*l);
							i2=sin(C_W*l);
							x[p][q].real+=(r1*r2+i1*i2);
							x[p][q].imaginario+=(i1*r2-r1*i2);
						}
					}
				}
				short *muestras1=malloc(sizeof(short)*numMuestrasAudio1*2);
				int indice,y;

				for(unsigned int l=0;l<L;l++){
					for(unsigned int m=1;m<=M;m++){
						indice=2*(l*M+m);
						y=m-1;	
						muestras1[indice-2]=(short)x[l][y].real;
						muestras1[indice-1]=(short)x[l][y].imaginario;		
					}	
				}
				fwrite(muestras1,bytesAudio,1,archivoWavSalida);
				fwrite(pie,bytesPie,1,archivoWavSalida);
				fclose(archivoWavSalida);
				fclose(archivoWavEntrada1);

		}else{
				printf("No se pudo crear el archivo de salida\n");
				return 1;
		}
	}	
	return 0;
}


