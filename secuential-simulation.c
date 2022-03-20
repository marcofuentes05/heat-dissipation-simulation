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
#include <stdio.h>
#include <string.h>
#include <time.h>

// function to print in console the array
void printArray(long double array[], int length) {
	printf("{");
	for (int i = 0 ; i < length; i++) {
		printf("%f, ", array[i]);		
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
	double total_time = 10;

    clock_t c_clock;

	// printf("Ingrese el numero de intervalos discretos: ");
	// scanf("%d", &n);
	// printf("Ingrese la temperatura inicial de toda la barra: ");
	// scanf("%lf", &t_0);
	// printf("Ingrese la temperatura en la frontera inzquierda: ");
	// scanf("%lf", &t_l);
	// printf("Ingrese la temperatura en la frontera derecha: ");
	// scanf("%lf", &t_r);
    // printf("Ingrese el tiempo total en segundos: ");
	// scanf("%lf", &total_time);
	// División del dominio en intervalos discretos
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
    // Vector solución con las temperaturas de la barra
    printf("CFL Stability Condition: %f\n", dt * C / (dx * dx));
    printf("SECUENTIAL VERSION\n");
    printf("DISCRETE_STEPS: %d\n", n);
    printf("BAR STATE: { %d, %d, %d, ... %d, %d, %d, ... %d, %d, %d }\n", (int)newTemperature[0], (int)newTemperature[1], (int)newTemperature[2], (int)newTemperature[n/2-1], (int)newTemperature[n/2], (int)newTemperature[n/2+1], (int)newTemperature[n-3], (int)newTemperature[n-2], (int)newTemperature[n-1]);
    printf("While loop time ellapsed: %f\n", ((float)c_clock)/CLOCKS_PER_SEC);
}
