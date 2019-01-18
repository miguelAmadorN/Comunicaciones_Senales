
./dft.exe coseno.wav fdt_coseno.wav

./dft.exe respuestaImpulso10.wav fdt_respuestaImpulso10.wav

./multiplicacion.exe fdt_respuestaImpulso10.wav fdt_coseno.wav multiplicacion10.wav

./idft.exe multiplicacion10.wav convolucion10.wav

