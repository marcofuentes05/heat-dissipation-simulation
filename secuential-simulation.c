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
#define L 5			// Length of the bar
#define C 10e-5		// Thermal Diffusivity of the Material
#define DT 0.5 
#define DX 0.05
#include <stdio.h>
#include <string.h>
#include <time.h>

// function to print in console the array
void printArray(double array[], int length) {
	printf("{");
	for (int i = 0 ; i < length; i++) {
		printf("%f, ", array[i]);		
	}
	printf("}\n");
}

// main function
int main(int argc, char* argv[]) {
	double err = ERR;
	int n = N;
	double length = L;
	double t_0 = 21;
	double t_l = 1;
	double t_r = 50;
	double dt = 0.005;
	double total_time = 500;

    clock_t c_clock;

	printf("Ingrese la precision: ");
	scanf("%lf", &err);
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
	double dx = length / n;
	double previousTemperature[n];
	double newTemperature[n];
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
    dt = err * dx * dx / C;
    c_clock = clock();
	while(currentDt < total_time) {
			for (int j = 1; j<n-1; j++) {
				// Calcular la nueva temperatura Tj(ti+1)
				newTemperature[j]=	previousTemperature[j] + C * (dt / (dx * dx)) * (previousTemperature[j-1] - 2 * previousTemperature[j] + previousTemperature[j+1]); 
			}
			for (int j = 0 ; j < n; j++) {
				// Actualizar el vector de solución Ti+1
				previousTemperature[j] = newTemperature[j];
			}
		currentDt += dt;
	}
    c_clock = clock() - c_clock;
    printf("While loop time ellapsed: %f\n", ((float)c_clock)/CLOCKS_PER_SEC);
    printf("dt=%f\n", dt);
		// Time 
		// Vector solución con las temperaturas de la barra
    printf("Time: %f\t", currentDt);
    printArray(newTemperature, n);
}
