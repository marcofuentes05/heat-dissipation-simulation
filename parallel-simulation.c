/*-------------------------
 *  secuential-simulation.c
 *-------------------------
 * Simulacion secuencial discreta de la ec. de disipacion de calor
 * en una barra metalica unidimensional de longitud L y c=10e-5
 */

#define ERR 0.05	    // Precision requerida
#define N  10		    // # de intervalos discretos
#define T_0 21		    // Temperatura inicial de la barra
#define T_L 21		    // Temperatura inicil en x=0
#define T_R 30		    // Temperatura inical en x=L
#define L 50			// Length of the bar
#define C 10e-5		    // Thermal Diffusivity of the Material
#define DT 0.5 
#define DX 0.05
#define NUM_THREADS 8   //Num. of threads in use
#include <stdio.h> 
#include <string.h>
#include <time.h>
#include <omp.h>
#include <stdlib.h>

// function to print in console the array
void printArray(long double array[], int length) {
	printf("{");
	for (int i = 0 ; i < length; i++) {
		printf("%Lf, ", array[i]);		
	}
	printf("}\n");
}

// main function
int main(int argc, char* argv[]) {
    // Default Values
	int n = 80000;
	double length = L;
	double t_0 = 20;
	double t_l = 10;
	double t_r = 80;
	double dt = 0.005;
	double total_time = 50;

	printf("Ingrese el numero de intervalos discretos: ");
	scanf("%d", &n);
	printf("Ingrese la temperatura inicial de toda la barra: ");
	scanf("%lf", &t_0);
	printf("Ingrese la temperatura en la frontera inzquierda: ");
	scanf("%lf", &t_l);
	printf("Ingrese la temperatura en la frontera derecha: ");
	scanf("%lf", &t_r);
    printf("Ingrese el tiempo total en segundos: ");
	scanf("%lf", &total_time);
	// División del dominio en intervalos discretos
    printf("Starting execution\n");
	double dx = length / n;
    double * prviousTemperature;
	long double previousTemperature[n];
	long double newTemperature[n];
	// Ajustar los valores iniciales de los vectores de solución Ti y Ti+1, para tiempo i
	for(int i =0; i<n; i++) {
		// Inicializamos el vector de temperaturas
		previousTemperature[i] = t_0;
		newTemperature[n] = t_0;
	}
	previousTemperature[0] = t_l;
	newTemperature[0] = t_l;
	previousTemperature[n-1] = t_r;
	newTemperature[n-1] = t_r;
    dt = 0.00125;
	double currentDt = total_time / dt;
    printf("currentDT: %f\n", currentDt);
    double t_initial = omp_get_wtime();
    long double *previousTemperaturePointer;
    long double *newTemperaturePointer;
    previousTemperaturePointer = previousTemperature;
    newTemperaturePointer = newTemperature;
    #pragma omp parallel num_threads(8) firstprivate(currentDt)
	while(currentDt--) {
        #pragma omp for schedule(static , 1000)
        for(int i = 1; i < n-1; i++) {
            newTemperaturePointer[i] = previousTemperaturePointer[i] + C * (dt / (dx * dx)) * (previousTemperaturePointer[i-1] - 2 * previousTemperaturePointer[i] + previousTemperaturePointer[i+1]);
        }
        #pragma omp single 
        {
            long double *temporalTemperature;
            temporalTemperature = previousTemperaturePointer;
            previousTemperaturePointer = newTemperaturePointer;
            newTemperaturePointer = temporalTemperature;
        }
	}
    double t_final = omp_get_wtime();
    printf("dt=%f\n", dt);
    printf("CFL Stability Condition: %f\n", dt * C / (dx * dx));
    printf("NUM_THREADS: %d\n", NUM_THREADS);
    printf("DISCRETE_STEPS: %d\n", n);
    printf("BAR STATE: { %0.2f, %0.2f, %0.2f, ... %0.2f, %0.2f, %0.2f, ... %0.2f, %0.2f, %0.2f }\n", newTemperaturePointer[0], newTemperaturePointer[1], newTemperaturePointer[2], previousTemperature[n/2-1], previousTemperature[n/2], previousTemperature[n/2+1], previousTemperature[n-3], previousTemperature[n-2], previousTemperature[n-1]);
    printf("While loop time ellapsed: %f\n", t_final - t_initial);
}
