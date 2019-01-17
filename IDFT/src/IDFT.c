#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.141592653589793

typedef struct CabeceraWAV{
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
	unsigned int Subchunk2_Size;//Catidad de bytes de informacion de la seal 
}Cabecera;

void informacionAudio(Cabecera cabeceraAudio){
	printf("%c%c%c%c\n",cabeceraAudio.ChunkID[0],cabeceraAudio.ChunkID[1],cabeceraAudio.ChunkID[2],cabeceraAudio.ChunkID[3]);
	printf("ChunkSize: %d\n",cabeceraAudio.ChunkSize);
	printf("%c%c%c%c\n",cabeceraAudio.Format[0],cabeceraAudio.Format[1],cabeceraAudio.Format[2],cabeceraAudio.Format[3]);
	printf("%c%c%c%c\n",cabeceraAudio.Subchunk1_ID[0],cabeceraAudio.Subchunk1_ID[1],cabeceraAudio.Subchunk1_ID[2],cabeceraAudio.Subchunk1_ID[3]);
	printf("Subchunk1_Size:%d   %d%d%d\n",cabeceraAudio.Subchunk1_Size[0],cabeceraAudio.Subchunk1_Size[1],cabeceraAudio.Subchunk1_Size[2],cabeceraAudio.Subchunk1_Size[3]);
	printf("Formato del audio: %d\n",cabeceraAudio.AudioFormat[0] );
	printf("Numero de canales: %d\n",cabeceraAudio.NumChannels[0] );
	printf("Muestras por segundo %d\n",cabeceraAudio.SampleRate);
	printf("Bytes por segundo: %d\n",cabeceraAudio.ByteRate);
	printf("Numero de bytes por muestra %d\n",cabeceraAudio.BlockAlign );
	printf("Numero de bits por muestra: %d\n", cabeceraAudio.BitsPerSample);
	printf("%c%c%c%c\n",cabeceraAudio.Subchunk2_ID[0],cabeceraAudio.Subchunk2_ID[1],cabeceraAudio.Subchunk2_ID[2],cabeceraAudio.Subchunk2_ID[3]);
	printf("Numero de bytes de la informacion %d\n",cabeceraAudio.Subchunk2_Size);
	printf("Numero de muestras %d\n",cabeceraAudio.Subchunk2_Size/(cabeceraAudio.NumChannels[0]*(cabeceraAudio.BitsPerSample/8)));
	printf("Bytes de sobra (informaciondesconocida): %d\n",cabeceraAudio.ChunkSize-36-cabeceraAudio.Subchunk2_Size);
	printf("Tam total del archivo: %d\n",cabeceraAudio.ChunkSize+8);	
}

//Resegra el tamao completo de el archivo en bytes
unsigned int getFileSize(Cabecera CW){
	return CW.ChunkSize+8;
}

//Regresa el tamao de las muestras en bytes
char getSampleSize(Cabecera CW){
	return (CW.BitsPerSample/8);	
}

//Regresa el numero de canales;
char getCannalNumber(Cabecera CW){
	return CW.NumChannels[0];
}


//Regresa el numero de muestras de audio si es momo o estero regresa la misma cantidad por lo cual en estereo se debe multiplicar por 2
unsigned int getNumberAudioSamples(Cabecera CW){
	return CW.Subchunk2_Size/(CW.NumChannels[0]*(CW.BitsPerSample/8));
}

//Regresa el tamanio total del archivo wav-8 
unsigned int getNumberBytesAudioInformation(Cabecera CW){
	return CW.Subchunk2_Size;
}


//Convierte una cabecera de tipo mono a una de tipo estereo
Cabecera setAudioStereo(Cabecera CW){
	CW.ChunkSize+=CW.Subchunk2_Size;
	CW.Subchunk2_Size*=2;
	CW.NumChannels[0]=2;
	CW.ByteRate*=2;
	CW.BlockAlign*=2;
	return CW;
}


//regresa el numero de bytes del pie del archivo wav
unsigned int getNumberBytesFoot(Cabecera CW){
	return CW.ChunkSize-36-CW.Subchunk2_Size;
}

//Regresa la cabecera del archvo wav
Cabecera getHeader(char *archivoWavEntrada){
	Cabecera CW;
	FILE *archivo=fopen(archivoWavEntrada,"r");
	fread(&CW,44,1,archivo);
	fclose(archivo);
	return CW;
}

//Regresa las muestras de un archivo con tamanio de 8 bits
char* getAudioSamples8bits(char *archivoWavEntrada){
	Cabecera CW;
	FILE *archivo=fopen(archivoWavEntrada,"r");
	fread(&CW,44,1,archivo);
	unsigned int numeroMuestras=CW.Subchunk2_Size/(CW.NumChannels[0]*(CW.BitsPerSample/8));
	char *Samples=malloc(sizeof(char)*numeroMuestras);
	fread(Samples,numeroMuestras,1,archivo);
	fclose(archivo);
	return Samples;
}	

//Regresa las muestras de audio de 16 bits mono
short* getAudioSamples16bits(char *archivoWavEntrada){
	Cabecera CW;
	FILE *archivo=fopen(archivoWavEntrada,"r");
	fread(&CW,44,1,archivo);
	unsigned int numeroMuestras=CW.Subchunk2_Size/(CW.NumChannels[0]*(CW.BitsPerSample/8));
	short *Samples=malloc(sizeof(short)*numeroMuestras);
	fread(Samples,numeroMuestras*2,1,archivo);
	fclose(archivo);
	return Samples;
}
short* getAudioSamples16bitsStereo(char *archivoWavEntrada){
	Cabecera CW;
	FILE *archivo=fopen(archivoWavEntrada,"r");
	fread(&CW,44,1,archivo);
	unsigned int numeroMuestras=CW.Subchunk2_Size/(CW.NumChannels[0]*(CW.BitsPerSample/8));
	numeroMuestras*=2;
	short *Samples=malloc(sizeof(short)*numeroMuestras);
	fread(Samples,numeroMuestras*2,1,archivo);
	fclose(archivo);
	return Samples;
}

//Regresa las muestras de 16bits del canal1
short* getAudioSamples16bisCanal1(char *archivoWavEntrada){
	Cabecera CW=getHeader(archivoWavEntrada);
	if(getCannalNumber(CW)==2){
		short *muestrasAudio=getAudioSamples16bitsStereo(archivoWavEntrada);
		unsigned int tam=getNumberAudioSamples(CW);
		short *canal1=malloc(tam*sizeof(short));
		for(unsigned int i=0;i<tam;i++){
			canal1[i]=muestrasAudio[2*i];
		}
		return canal1;
	}else{
		printf("El archivo: %s no es estereo\n",archivoWavEntrada);
		return NULL;
	}
}

//Regresa las muestras de 16bits del canal2
short* getAudioSamples16bisCanal2(char *archivoWavEntrada){
	Cabecera CW=getHeader(archivoWavEntrada);
	if(getCannalNumber(CW)==2){
		short *muestrasAudio=getAudioSamples16bitsStereo(archivoWavEntrada);
		unsigned int tam=getNumberAudioSamples(CW);
		short *canal2=malloc(tam*sizeof(short));
		for(unsigned int i=0;i<tam;i++){
				canal2[i]=muestrasAudio[2*i+1];
		}
		return canal2;
	}else{
		printf("El archivo: %s no es estereo\n",archivoWavEntrada);
		return NULL;
	}
}



//Regresa las bytes del pie del archivo wav
char* getFileFoot(char *archivoWavEntrada){
	Cabecera CW;
	FILE *archivo=fopen(archivoWavEntrada,"r");
	fread(&CW,44,1,archivo);
	unsigned int bytesSobrantes=CW.ChunkSize-CW.Subchunk2_Size-36;
	char *bytes=malloc(sizeof(char)*CW.Subchunk2_Size);
	fread(bytes,CW.Subchunk2_Size,1,archivo);
	bytes=malloc(sizeof(char)*bytesSobrantes);
	fread(bytes,bytesSobrantes,1,archivo);
	return bytes;
}
int main(int argc, char *argv[]){

	FILE *archivoWavEntrada;
	archivoWavEntrada=fopen(argv[1],"r");

	if(archivoWavEntrada==NULL){
		printf("Error en la lectura del archivo de audio\n");
	}else{
		Cabecera CW=getHeader(argv[1]);

		if(getCannalNumber(CW)==2){
			FILE *archivoWavSalida=fopen(argv[2],"w");

			if(archivoWavSalida!=NULL){
			
				fwrite(&CW,44,1,archivoWavSalida);
				short *muestrasAudioCanal1=getAudioSamples16bisCanal1(argv[1]);
				short *muestrasAudioCanal2=getAudioSamples16bisCanal2(argv[1]);

				int numMuestrasAudio=getNumberAudioSamples(CW);//Canal1 y canal 2
				double *muestrasAudioSalida=malloc(sizeof(double)*numMuestrasAudio*2);
				short *muestrasAudioFinal=malloc(sizeof(short)*numMuestrasAudio*2);//incluira ambos canales

   
				double C=2*PI/numMuestrasAudio;
				for(int n=0;n<numMuestrasAudio;n++){
					muestrasAudioSalida[2*n]=0;
					muestrasAudioSalida[2*n+1]=0;
					for(int k=0;k<numMuestrasAudio;k++){	//Parte Real
						muestrasAudioSalida[2*n]+=((muestrasAudioCanal1[k]*cos(C*k*n))-(muestrasAudioCanal2[k]*sin(C*k*n)));
					}

					for(int k=0;k<numMuestrasAudio;k++){	//Parte Imaginaria
						muestrasAudioSalida[2*n+1]+=((muestrasAudioCanal1[k]*sin(C*k*n))+(muestrasAudioCanal2[k]*cos(C*k*n)));
					}
				}


				numMuestrasAudio*=2;
				for(int i=0;i<numMuestrasAudio;i++){
					muestrasAudioFinal[i]=muestrasAudioSalida[i];
				}
		
				unsigned int bytesAudio=getNumberBytesAudioInformation(CW);

				fwrite(muestrasAudioFinal, bytesAudio,1,archivoWavSalida);
				char *pie=getFileFoot(argv[1]);
				fwrite(pie,getNumberBytesFoot(CW),1,archivoWavSalida);
				fclose(archivoWavSalida);
				fclose(archivoWavEntrada);
				return 0;

			}else{
				printf("No se pudo crear el archivo de salida\n");
				return 0;
			}	
		}else{
			printf("El archivo el monoaural\n");
			return 0;
		}

	}
}
