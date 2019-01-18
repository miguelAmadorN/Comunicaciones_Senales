
./dft.exe coseno.wav fdt_coseno.wav

./dft.exe respuestaImpulso30.wav fdt_respuestaImpulso30.wav

./multiplicacion.exe fdt_respuestaImpulso30.wav fdt_coseno.wav multiplicacion30.wav

./idft.exe multiplicacion30.wav convolucion30.wav

