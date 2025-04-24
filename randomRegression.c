#include <stdio.h>		// stdio per funzioni standard
#include <stdlib.h>		// stdlib per il rand()
#include <math.h>		// math per fabs()
#include <time.h>		// time per impostare casualemnte il seed del rand

#define N 10        // N = numero di punti
#define EPOCHS 15   // EPOCHS = numero di epoche, iterazioni

// retta
typedef struct {                    // struct retta, contiene le informazioni riguardo la retta
    double m, q;                    // m = coefficiente angolare, q = intercetta
    double mbe, mae, mse;           // mbe = mean bias error, mae = mean absolute error, mse = mean squared error
} retta;

// MBE
double MBE (double *y, double *y_pred, int n) {    	// *... == ...[], array 
                                                        // y_true = y effettiva, la y effettiva è quella dei punti
                                                        // y_pred = y predetta, la y predetta è quella che si trova sostituendo la x dei punti nella retta generata
                                                        // n = numero di punti

    double sum = 0.0;                                   // imposto la somma = 0
    for (int i = 0; i < n; i++) {                       // valuto tutti i punti dal primo al decimo (posizione 0 to 9)
        sum += (y_pred[i] - y[i]);                 	// aggiungo alla somma la differenza tra predetto e effettivo
    }
    return sum / n;                                     // restituisco la media (mean)
}

// MAE
double MAE (double *y, double *y_pred, int n) {
    double sum = 0.0;                                   // imposto la somma = 0
    for (int i = 0; i < n; i++) {                       // valuto tutti i punti dal primo al decimo (posizione 0 to 9)
        sum += fabs(y_pred[i] - y[i]);             	// fabs(...) si usa per trovare il valore assoluto di un float o double
                                                        // aggiungo alla somma il valore assoluto della differenza tra effettivo e quadrato
    }
    return sum / n;                                     // restituisco la media (mean)
}

// MSE
double MSE (double *y, double *y_pred, int n) {
    double sum = 0.0;                                           // imposto la somma = 0
    for (int i = 0; i < n; i++) {                               // valuto tutti i punti dal primo al decimo (posizione 0 to 9)
        sum += (y_pred[i] - y[i]) * (y_pred[i] - y[i]);   	// aggiungo alla somma la differenza tra predetto e effettivo al quadrato
    }
    return sum / n;                                             // restituisco la media (mean)
}

// valuto la retta
void valutatore(double *x, double *y, double m, double q, retta *retta, int n) {	// passo tutti i punti *x, *y, arrays (puntatore al primo elemento)
																					// passo il coefficiente angolare m e intercetta q della retta da esaminare
																					// passo il riferimento alla retta in esame (puntatore alla struct retta in esame)
																					// passo il numero di punti n
	
    double y_pred[N];				// creo un array che abbia le y predette
    for (int i = 0; i < n; i++) {	
        y_pred[i] = m * x[i] + q;		// calcolo le y predette in base alle x reali
    }
    
    retta->m = m;				// imposto la m della retta che sto puntando adesso, quella passata alla funzione
						// sto inserendo i dati della retta nella struct. e' come dire *retta.m = m
    retta-> q = q;				// imposto q allo stesso modo
    retta->mbe = MBE(y, y_pred, n);		// imposto MBE della retta allo stesso modo. MBE calcolato dalla funzione MBE(...)
    retta->mae = MAE(y, y_pred, n);		// imposto MAE della retta allo stesso modo. MAE calcolato dalla funzione MAE(...)
    retta->mse = MSE(y, y_pred, n);		// imposto MSE della retta allo stesso modo. MSE calcolato dalla funzione MSE(...)
}

int main() {

    srand(time(0));									// imposto in base all'ora corrente il seed del rand per rendere i numeri casuali
	
    double x[N] = {0.00, 1.11, 2.22, 3.33, 4.44, 5.56, 6.67, 7.78, 8.89, 10.00};	// punti casuali con x equidistanti tra 0 e 10
    double y[N] = {-7.13, -2.44, -2.35, -1.52, -3.03, 2.61, 1.52, 4.28, 7.62, 9.81};	// y casuali comprese tra 10 e -10

    retta retta_migliore = {0.0, 0.0, 0.0, 0.0, 1e9};					// imposto una retta fittizia con errore massimo in modo che venga sicuramente sostituita
    retta retta_attuale;

    for (int i = 0; i < EPOCHS; i++) {							// inizio a generare rette (in corrispondenza al numero di iterazioni o epoche)

        double m = ((double)rand() / RAND_MAX) * 2.0 - 1;       			// genero un numero tra -1 e 1, il numero generato è compreso tra 0 e RAND_MAX,
                                                                			// se divido per RAND_MAX ottengo al massimo 1. moltiplico il numero generato,
                                                               				// compreso tra 0 e 1, per 2 e poi sottraggo per 1.
        double q = ((double)rand() / RAND_MAX) * 2.0 - 1;				// faccio lo stesso per l'intercetta
        
        valutatore(x, y, m, q, &retta_attuale, N);					// passo la retta alla funzione "valutatore" per valutarla e calcolare gli errori

        if (retta_attuale.mse < retta_migliore.mse) {					// scelgo la retta con MSE minore (avrei potuto farlo in alternativa con MBE o MAE)
            retta_migliore = retta_attuale;						// se MSE è il minore sceglo la retta
        }
    }
    
    // stampo tutti i dati della retta migliore
    printf("La retta migliore e':\n");
    
    if (retta_migliore.q >= 0) {							// per formattazione (q può essere negativa e quindi viene 3.1x + -0.83)
		printf("y = %.2fx + %.2f\n", retta_migliore.m, retta_migliore.q);	// stampo la retta in forma esplicita
	} else {
		printf("y = %.2fx %.2f\n", retta_migliore.m, retta_migliore.q);
	}
    printf("MBE: %.4f\n", retta_migliore.mbe);						// stampo MBE (non è detto che non ne sia stato generato uno migliore)
    printf("MAE: %.4f\n", retta_migliore.mae);						// stampo MAE (non è detto che non ne sia stato generato uno migliore)
    printf("MSE: %.4f\n", retta_migliore.mse);						// stampo MSE (è il migliore)

    return 0;
}
