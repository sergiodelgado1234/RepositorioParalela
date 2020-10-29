# OpenMP Examples

OpenMP es una biblioteca para crear programas paralelos y portables, utilizando memoria compartida.

Existen diferentes soluciones para programacion paralela

1. Biblioteca de hilos
- WinAPI / POSIX Threads (memoria compartida)
2. Directivas al compilador
- OpenMP (memoria compartida)
3. Biblioteca de pasajes de mensajes
- MPI (memoria distribuida)

## Caracteristicas de OpenMP

- Basados en directivas al compilador (#pragma)
- Se encarga de la creacion/sincronizacion/destruccion de hilos
- Agrega funciones especificas
- Se puede utilizar para paraleliza antiguo codigo secuencial
- Se basa en el modelo fork- join
- No requiere instalacion en Linux

Las directivas de OpenMP se aplican a bloques de codigo, o a sentencias individuales. En C la forma de especificar bloques de codigos es usando llaves

```
void main(){
	//bloque de codigo del main

	{
		//bloque de codigo anonimo
	}	
}
		
```
    
 Para compilar utilizando openmp:   
 
 `gcc programa.c -o programa -fopenmp`
    
 Para incluir la libreria se llama:  
    
  `#include <omp.h>`
  
  
  ###### Tipos de funciones
  
1. Definicion de region paralela
2. Worksharing
3. Ambiente de datos
4. Sincronizacion

Funciones relacionadas con cantidad de hilos:

```int omp_get_thread_num();  // nos devuelve el ID del thread
int omp_set_num_thread();  // establecemos cantidad de threads
int omp_get_num_thread();  // nos devuelve cantidad de threads```

 ###### Definicion de una region paralela
 
 Iniciar una region paralela implica que en tiempo de ejecucion se creara un grupo de hilos. Por defecto se toma del sistema operativo la cantidad de hilos disponibles para la tarea. Adicionalmente podemos especificar la cantidad de hilos explicitamente a traves de codigo o parametros por consola. En codigo, definir una region paralela:
 
 ```#include <stdio.h>
#include <omp.h>

void main(int argc, char const *argv[])
{
	//con 4 hilos
    omp_set_num_threads(4);
    //declaramos una region paralela 
    #pragma omp parallel
    {
    	//muestra el id del hilo que se esta ejecutando
        printf("%d\n", omp_get_thread_num());
    }
}```

 ## Worksharing 
 
 Comparte el trabajo de un bucle for entre distintos hilos. Los indices i que le tocan a cada hilo son privados.
Consideraciones: las variables automaticas creadas durante la ejecucion de los threads son privadas del thread correspondiente. Los datos que son almacenados en el heap son compartidos. La variable de control del loop sobre el que se especifica un #pragma omp for es privada de cada thread. La mayoria de las variables pueden ser declaradas explicitamente como privadas o compartidas en una directiva. Sintaxis:

```#include <stdio.h>
#include <omp.h>

#define LARGO 1000

int main(int argc, char const *argv[])
{
    int valores[LARGO];

    #pragma omp parallel
    {
        #pragma omp for
        for (int i = 0; i < LARGO; ++i)
        {
            valores[i] = i;
        }
    }
    return 0;
}
```
   
## Ambiente de datos
 
- Clausulas para #pragma omp for
  - private(list)
  - reduction(operator:list)
  - schedule(kind[chunk_size])
 
 ejemplo, calcular PI
 
 ```
#include <stdio.h> 
#include <omp.h>


static long num_steps = 100000;
double step;
void main (){ 
    int i; double x, pi, sum = 0.0;
    step = 1.0/(double) num_steps;
    
    #pragma omp parallel
    {
        #pragma omp for private(x,i) reduction(+:sum)
        for (i=0;i< num_steps; i++){
            #pragma omp critical
            x = (i+0.5)*step;
            sum = sum + 4.0/(1.0+x*x);
        }       
    }
    pi = step * sum;
    printf("%e \n",pi);
}
```
 
 
 ##  Sincronizacion
 
 Barreras y locks –
- Todos los threads deben llegar a este punto para después seguir ejecutando
 
 `#pragma omp barrier `
 
 
 - Inicializa un lock que simpre queda en estado unlocked
 
  `void omp_init_lock(omp_lock_t *lock);`
  
 - Se apropia (o intenta apropiarse) de un lock
 
 `void omp_set_lock(omp_lock_t *lock);`
 
 - Destruye el lock
 
 `void omp_destroy_lock(omp_lock_t *lock); `
 
 
 
 
  
