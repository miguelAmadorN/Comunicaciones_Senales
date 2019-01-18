# Muestreo y Reconstrucción de una señal

De manera teorica una señal se muestrea multiplicando contra un tren de impulsos.

![Muestreo](../Muestreo_Y_Reconstruccion/muestreo.png) 

Para reconstruir una señal que fue muestreada, necesitamos pasarla al dominio de la
frecuencia y multiplicarla contra un filtro paso bajo ideal del fs/2.

Fs: Frecuencia de muestreo. 

![Reconstruccion](../Muestreo_Y_Reconstruccion/reconstruccion.png) 

Una vez multiplicada en la fecuencia aplicamos la IDFT. 

**Caracteristicas**

DFT con la fórmula (escalado al valor máximo y con perdida de información):

![DFT](../ConvolucionEnLaFrecuencia/formula_dft.png)

IDFT con la fórmula (aplicando aquí el promedio y no en la DFT)

![DFT](../ConvolucionEnLaFrecuencia/formula_idft.png)

La multiplicación (con perdida de información, sin escalar al valor máximo)


**Ejemplo de ejecución**

Señal

![Señal](../Muestreo_Y_Reconstruccion/senial.png) 


Tren de Impulsos

![Tren de impulsos](../Muestreo_Y_Reconstruccion/tren_impulsos.png) 


Señal muestreada

![Señal](../Muestreo_Y_Reconstruccion/senial_muestreada.png) 


Señal muestreada en frecuencia

![Señal en frecuencia](../Muestreo_Y_Reconstruccion/senial_en_frecuencia.png) 


Filtro ideal

![Filtro](../Muestreo_Y_Reconstruccion/filtro_ideal.png) 


Multiplicación de filtro y señal en frecuencia

![Producto](../Muestreo_Y_Reconstruccion/producto.png) 


Señal reconstruida

![Reconstruccion](../Muestreo_Y_Reconstruccion/senial_reconstruida.png) 


Mas información: https://www.youtube.com/watch?v=nSGQ8fJjJUw


