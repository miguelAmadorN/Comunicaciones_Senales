
./dft.exe coseno.wav fdt_coseno.wav

./dft.exe respuestaImpulso20.wav fdt_respuestaImpulso20.wav

./multiplicacion.exe fdt_respuestaImpulso20.wav fdt_coseno.wav multiplicacion20.wav

./idft.exe multiplicacion20.wav convolucion20.wav

