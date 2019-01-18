# Transformada de Fourier Discreta (DFT)

La Transformada de Fourier Discreta permite pasar un señal del tiempo 
a la frecuencia.

Implementado con la fórmula:

$ X_{k} = frac{1}{N}\sum_{n = 0}^{N - 1}X_{n} e^{ - frac{2PIi}{N} kn} K = 0, ... , N - 1$


**Ejecutar**

./DFT [nombre wav entrada] [nombre wav salida]

Ejemplo de ejecucion: 

![Seno](../DFT/sin5.png)

![DFT Seno](../DFT/dftsin5.png)

![Coseno](../DFT/cos5.png)

![DFT Coseno](../DFT/dftcos5.png)
