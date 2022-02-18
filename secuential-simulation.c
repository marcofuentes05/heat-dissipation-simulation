/*-------------------------
 *  secuential-simulation.c
 *-------------------------
 * Simulacion secuencial discreta de la ec. de disipacion de calor
 * en una barra metalica unidimensional de longitud L y c=10e-5
 */

#define ERR 0.05	// Precision requerida
#define N  10e4		// # de intervalos discretos
#define T_0 21		// Temperatura inicial de la barra
#define T_L 0		// Temperatura inicial en x=0
#define T_R 30		// Temperatura inicla en x=L

double finalTemperature(double time, double x);

int main(int argc, char* argv[]) {
	double err = ERR;
	int n = N;
	double t_0 = T_0;
	double t_l = T_L;
	double t_r = T_R;
}

double finalTemperature(double time, double x) {
	
}
