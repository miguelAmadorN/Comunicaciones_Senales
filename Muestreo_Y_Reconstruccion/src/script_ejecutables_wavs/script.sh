
./multiplicacion.exe trenImpulsos.wav cos.wav muestreo.wav 
echo paso1 terminado

./dft.exe muestreo.wav fdt_coseno.wav 
echo paso2 terminado

./multiplicacion.exe si.wav fdt_coseno.wav multiplicacion.wav 
echo paso3 terminado

./idft.exe multiplicacion.wav reconstruccion.wav
echo paso4 terminado

echo FIN
