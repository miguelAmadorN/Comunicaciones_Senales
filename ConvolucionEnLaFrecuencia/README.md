# Simulación de Circuito RC

Filtro pasa bajas simulando un circuito RC, multiplicando las señales en la
frecuencia, lo que es equivalente a hacer la convolución en el tiempo.

Proceso:
Aplicar la DFT a la señal que se aplicaré el filtro pasa bajas.
Aplicar la DFT a la respuesta al impulso.
Multiplicar las señales en la frecuencia.
Aplicar la IDFT a la producto de las señales en frecuencia.  


