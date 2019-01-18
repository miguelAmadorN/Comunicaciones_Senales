#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <alsa/asoundlib.h>
#define MAX 26
#define device "default"
#define PI 3.141592653589793

char* getTam(){
 	char *tam = (char*) malloc(sizeof(char) * 4);
 	char t    = MAX - 4;
 	tam[0] = (char)((t >> 24) & 0xff);
 	tam[1] = (char)((t >> 16) & 0xff);
 	tam[2] = (char)((t >>  8) & 0xff);
 	tam[3] = (char)( t & 0xff);
 	return tam;
}

char* getScratchCommand(char nota)
{
	char *tam     = getTam();
	char *mensaje = (char *) malloc(sizeof(char) * MAX);
	mensaje[0]    = tam[0];
	mensaje[1]    = tam[1];
	mensaje[2]    = tam[2];
	mensaje[3]    = tam[3];
	mensaje[4]    = (char)'s';
	mensaje[5]    = (char)'e';
	mensaje[6]    = (char)'n';
	mensaje[7]    = (char)'s';
	mensaje[8]    = (char)'o';
	mensaje[9]    = (char)'r';
	mensaje[10]   = (char)'-';
	mensaje[11]   = (char)'u';
	mensaje[12]   = (char)'p';
	mensaje[13]   = (char)'d';
	mensaje[14]   = (char)'a';
	mensaje[15]   = (char)'t';
	mensaje[16]   = (char)'e';
	mensaje[17]   = (char)' ';
	mensaje[18]   = (char)'"';
	mensaje[19]   = (char)'n';
	mensaje[20]   = (char)'o';
	mensaje[21]   = (char)'t';
	mensaje[22]   = (char)'e';
	mensaje[23]   = (char)'"';
	mensaje[24]   = (char)' ';
	mensaje[25]   = (char)nota;
	return mensaje;
}


double absDouble(double numero)
{
	if(numero < 0)
		return numero * (-1);
	return numero;
}

int absNum(int numero)
{
	if(numero < 0)
		return numero * (-1);
	return numero;
}

char valor(char encontrados[])
{
	if(encontrados[0])
		return '1';//Do
	if(encontrados[1])
		return '2';//Re
	if(encontrados[2])
		return '3';//Mi
	if(encontrados[3])
		return '4';//Fa
	if(encontrados[4])
		return '5';//Sol
	if(encontrados[5])
		return '6';//La
	if(encontrados[6])
		return '7';//si
	return '0';

}
	      
short* DFT(short muestrasAudio[], unsigned int numMuestrasAudio)
{
	int    valorMaximoEntrada = absNum(muestrasAudio[0]);
	short  *muestrasAudio2S   = malloc(sizeof(short)  * numMuestrasAudio * 2);
	double *muestrasAudio2    = malloc(sizeof(double) * numMuestrasAudio * 2);
			
	for(int i = 1; i < numMuestrasAudio; i++)
	{
		if(valorMaximoEntrada < absNum(muestrasAudio[i]))
			valorMaximoEntrada = absNum(muestrasAudio[i]);
	}
	if(valorMaximoEntrada = 32768)
		valorMaximoEntrada--;

	for(int k=0;k<numMuestrasAudio;k++)
	{
		muestrasAudio2[2 * k]     = 0;
		muestrasAudio2[2 * k + 1] = 0;
		for(int n = 0; n < numMuestrasAudio; n++)
		{	
			muestrasAudio2[2 * k] += muestrasAudio[n] * cos(2 * PI * k * n / numMuestrasAudio);
		}
		for(int n = 0; n < numMuestrasAudio; n++)
		{
			muestrasAudio2[2 * k + 1] += muestrasAudio[n] * sin(2 * PI * k * n / numMuestrasAudio);
		}
		muestrasAudio2[2*k+1]*=-1;
	}

	double valorMaximo = absDouble(muestrasAudio2[0]);
	for(int i = 1; i < numMuestrasAudio * 2; i++)
	{
		if(valorMaximo < absDouble(muestrasAudio2[i]))
			valorMaximo = absDouble(muestrasAudio2[i]);
	}

	for(int i = 0; i < numMuestrasAudio * 2; i++)
	{
		muestrasAudio2S[i] = muestrasAudio2[i] * valorMaximoEntrada / valorMaximo;	
	}
	return muestrasAudio2S;
}

int main (int argc, char *argv[])
{
	int i;
	int err;
	char numChannels = 1;
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	unsigned int rate = 4000;
	unsigned int numMuestrasAudio = 200;
	short buf[numMuestrasAudio];
	
	if ((err = snd_pcm_open (&capture_handle, device, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
		fprintf (stderr, "cannot open audio device %s (%s)\n", device, snd_strerror (err));
		exit (1);
	}
		   
	if ((err = snd_pcm_hw_params_malloc (&hw_params)) < 0) {
		fprintf (stderr, "cannot allocate hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
			 
	if ((err = snd_pcm_hw_params_any (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot initialize hardware parameter structure (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	
	if ((err = snd_pcm_hw_params_set_access (capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
		fprintf (stderr, "cannot set access type (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	
	if ((err = snd_pcm_hw_params_set_format (capture_handle, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {
		fprintf (stderr, "cannot set sample format (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	
	if ((err = snd_pcm_hw_params_set_rate_near (capture_handle, hw_params,&rate, 0)) < 0) {
		fprintf (stderr, "cannot set sample rate (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	
	if ((err = snd_pcm_hw_params_set_channels (capture_handle, hw_params, numChannels)) < 0) {
		fprintf (stderr, "cannot set channel count (%s)\n",
			 snd_strerror (err));
		exit (1);
	}
	
	if ((err = snd_pcm_hw_params (capture_handle, hw_params)) < 0) {
		fprintf (stderr, "cannot set parameters (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	snd_pcm_hw_params_free (hw_params);

	if ((err = snd_pcm_prepare (capture_handle)) < 0) {
		fprintf (stderr, "cannot prepare audio interface for use (%s)\n",
			 snd_strerror (err));
		exit (1);
	}

	//Flauta
	double resolucion = (double)((double)rate / (double)numMuestrasAudio);
	char numFrec = 8;
	short Frecuencias[] ={ 523, 587, 659, 704, 784, 880, 980 };
	unsigned int posiciones[numFrec];
	char encontrados[numFrec];
	for(unsigned int i = 0; i < numFrec; i++){
		posiciones[i] = (unsigned int)(Frecuencias[i] / resolucion);	
	}
	
	//Socket
	int cd, error, bytes;
	char *ip   = "127.0.0.1";//cambiar IP
	int puerto = 42001;//cambiar puerto
	struct sockaddr_in servidorInfo;
	socklen_t tam;
	if((cd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("Error en la funcion socket()\n");
		return 1;
	}

	memset(&servidorInfo, 0, sizeof(servidorInfo));
	servidorInfo.sin_family      = AF_INET;
	servidorInfo.sin_addr.s_addr = inet_addr(ip);
	servidorInfo.sin_port        = htons(puerto);
	tam = sizeof(servidorInfo);

	while((error = connect(cd, (const struct sockaddr *) &servidorInfo, tam)) <0 )
	{
		printf("Conectando\r");
	}
	printf("CONECTADO\n");
	char *msj;
	char m[MAX];
	//
	char val;
	short *muestrasAudio2S;

	do 
	{
		if ((err = snd_pcm_readi (capture_handle, buf, numMuestrasAudio)) != numMuestrasAudio) 
		{
			fprintf (stderr, "read from audio interface failed (%s)\n",snd_strerror (err));
			exit (1);
		}
		muestrasAudio2S = DFT(buf,numMuestrasAudio);
		for(unsigned int i = 0; i <numFrec; i++)
		{
			if(absNum(muestrasAudio2S[2 * posiciones[i]]) > 32300 || 
			   absNum(muestrasAudio2S[2 * posiciones[i] + 1]) > 32300 || 
			   absNum(muestrasAudio2S[2 * (posiciones[i] + 1)]) > 32300 || 
			   absNum(muestrasAudio2S[2 * (posiciones[i] + 1) + 1]) > 32300)
			{
				encontrados[i]=1;
			}
			else
			{
				encontrados[i]=0;
			}
		}
		val = valor(encontrados);
		msj = getScratchCommand(val);
		for(char i = 0; i < MAX; i++)
			m[i] = msj[i];

		if((bytes = send(cd, m, MAX, 0)) < 0)
			printf("Error al enviar el mensaje");
			
	}while(1);
	
	snd_pcm_close (capture_handle);
	exit (0);
}
