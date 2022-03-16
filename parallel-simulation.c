/*-------------------------
 *  secuential-simulation.c
 *-------------------------
 * Simulacion secuencial discreta de la ec. de disipacion de calor
 * en una barra metalica unidimensional de longitud L y c=10e-5
 */

#define ERR 0.05	// Precision requerida
#define N  10		// # de intervalos discretos
#define T_0 21		// Temperatura inicial de la barra
#define T_L 21		// Temperatura inicil en x=0
#define T_R 30		// Temperatura inical en x=L
#define L 50			// Length of the bar
#define C 10e-5		// Thermal Diffusivity of the Material
#define DT 0.5 
#define DX 0.05
#define NUM_THREADS 32 //Num. of threads in use
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

double getNewTemperature(double c, double dt, double dx, double pj, double pj_1, double pj__1){
		return pj + c * (dt / (dx * dx)) * (pj_1 - 2 * pj + pj__1); 
}

void getLocalTemperature(long double* previousFragment, int previousFragmentSize, double dt, double dx, int offset, int returnSize, long double* newTemperature) {
    #pragma omp parallel for schedule(static) num_threads(8)
    for(int i = offset; i < returnSize+offset; i++) {
        if (i==0 || i == previousFragmentSize-1) {
            newTemperature[i] = previousFragment[i];
        } else {
		    newTemperature[i] = getNewTemperature(C, dt, dx, previousFragment[i], previousFragment[i-1], previousFragment[i+1]);
        }
	}
}

// main function
int main(int argc, char* argv[]) {
    // Default Values
	int n = 80000;
	double length = L;
	double t_0 = 20;
	double t_l = 10;
	double t_r = 30;
	double dt = 0.005;
	double total_time = 10;

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
	double currentDt = 0;
    dt = 0.000125;
    double t_initial = omp_get_wtime();
	while(currentDt < total_time) {
        #pragma omp parallel num_threads(NUM_THREADS)
        {
            int offset = omp_get_thread_num() * (n / NUM_THREADS);
            int returnSize = (int)((double)n/(double)NUM_THREADS);
            getLocalTemperature(previousTemperature, n, dt, dx, offset, returnSize , newTemperature);
        }
        for (int j = 0 ; j < n; j++) {
            // Actualizar el vector de solución Ti+1
            previousTemperature[j] = newTemperature[j];
        }
		currentDt += dt;
	}
    double t_final = omp_get_wtime();
    printf("dt=%f\n", dt);
    printf("CFL Stability Condition: %f\n", dt * C / (dx * dx));
    printf("NUM_THREADS: %d\n", NUM_THREADS);
    printf("DISCRETE_STEPS: %d\n", n);
    printf("BAR STATE: { %d, %d, %d, ... %d, %d, %d, ... %d, %d, %d }\n", (int)previousTemperature[0], (int)previousTemperature[1], (int)previousTemperature[2], (int)previousTemperature[n/2-1], (int)previousTemperature[n/2], (int)previousTemperature[n/2+1], (int)previousTemperature[n-3], (int)previousTemperature[n-2], (int)previousTemperature[n-1]);
    printf("While loop time ellapsed: %f\n", t_final - t_initial);
}
