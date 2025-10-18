#include <stdio.h>
#include <stdlib.h>
#include <math.h> //para usar a função random
#include <time.h>
#include "linked-list.h"

#define CHEGADA 0 //tipo de evento (no struct)
#define PARTIDA 1

#define LAMBDA 200 //taxa de chegada
#define MAX_samp 10000 //nº de samples

#define receivers 1 //nº de recetores
#define qeue 13 //tamanho da fila

int block = 0; //conta o nº de chamadas bloqueadas


typedef struct
{
    double delay_prob;
    double avg_delay;
    double p_delay_gt_Ax;
    double bin_width; 
    int num_bins; 
    int *histogram; // pointer for the histogram
} EC_Metrics;

// ----------------- Functions to calculate the teoretical Am -----------------
double calculate_factorial(int N) {
    double result = 1.0;
    for (int i = 1; i <= N; i++) {
        result *= i;
    }
    return result;
}

double calculate_p_delay(double lambda, double dm, int N){
    double A = lambda * dm;
    double numerator = (pow(A, N) / calculate_factorial(N)) * (N / (N - A));
    double denominator = 0.0;

    for (int k = 0; k < N; k++) {
        denominator += (pow(A, k) / calculate_factorial(k));
    }
    denominator += numerator;

    return (double)(numerator / denominator);
}

double calculate_p_block(double lambda, double dm, int N) {
    double A = lambda * dm;
    double numerator = (double)(pow(A, N) / calculate_factorial(N));
    double denominator = 0.0;

    for (int k = 0; k < N; k++) {
        denominator += (double)(pow(A, k) / calculate_factorial(k));
    }
    denominator += numerator;

    return (double)(numerator / denominator);
}

double calculate_am(double p_delay, double lambda, double p_block) {
    return (double)(p_delay / (lambda * (1 - p_block)));
}

// ------------------------------------------------------

EC_Metrics erlang_C(int lambda, double dm, int N, int max_samps, double Ax){
    int busy = 0;
	int arrivals = 0;
	int delayed = 0;
    int delayed_gt_Ax = 0;
    double sum_delay = 0.0;
    double delay = 0.0;

    double ev_time = 0.0;

    list * events_list = NULL;
	queue_list * queue = NULL;

    // ------------------- Histogram --------------------
    double p_delay = calculate_p_delay(lambda, dm, N);
    double p_block = calculate_p_block(lambda, dm, N);
    double max;
    if(N == 1) max = 3 * 60; // for N = 1 its not possible to estimate Am due to instability
    else max = 30 * calculate_am(p_delay, lambda, p_block); // this way the range of delays is better represented for each case 
	int intervals = 25;
    double delta = (double)(max/intervals);

    int *histogram = malloc(intervals * sizeof(int));
    for (int k = 0; k < intervals; k++) histogram[k] = 0;
    // ---------------------------------------------------

	/*
    events_list = generate_events(lambda, dm, CHEGADA, ev_time, events_list); // first event

    while (arrivals < max_samps){

        int ev_type = events_list->type;
        ev_time = events_list->time;

        if(events_list != NULL) events_list = __remove(events_list); // remove the event that passed

        if(ev_type == CHEGADA){
            arrivals++;
            
            if(busy < N){ // if is free the event is automatically attended
                busy++;
                events_list = generate_events(lambda, dm, PARTIDA, ev_time, events_list);
            }

            else {
                delayed++;
                queue = __add_queue(queue, ev_time); // we store the time the event entered the queue
            }
            // in both cases we generate the next arrival
            events_list = generate_events(lambda, dm, CHEGADA, ev_time, events_list);
        }

        else{ // Partida
            if(queue != NULL){ // if the queue is not empty the service continues as busy
                double ev_arrival;
                queue = __remove_queue(queue, &ev_arrival); // we are going to obtain the time the event entered the queue
                events_list = generate_events(lambda, dm, PARTIDA, ev_time, events_list); // now that the event got out the queue we can generate a new departure

                delay = ev_time - ev_arrival;

                // build the histogram:
                int i = (int)((delay)/delta);
                if (i >= intervals) i = intervals -1;
                histogram[i]++;

                // for the average delay calculation:
                sum_delay += delay;
                if (delay > Ax) delayed_gt_Ax++;
            }
            else busy--; 
        }
    }
	*/

    EC_Metrics results;
    results.delay_prob = ((double)delayed / (double)arrivals) * 100;
    results.avg_delay = (double)(sum_delay/arrivals);
    results.p_delay_gt_Ax = ((double)delayed_gt_Ax / (double)arrivals) * 100;

    // histogram:
    results.bin_width = delta;
    results.num_bins = intervals;
    results.histogram = histogram;

    return results;
}



int main(void)
{
	list  * event_list;
	int ev_type; 
	double ev_time = 0;
	event_list = NULL;
	
	//LAB1
	double c_t = 0; //para calcular a média de c
	double max = 5.0/LAMBDA; //nº max de valores representaveis 
	double delta = 1.0/(5.0*LAMBDA); 
	int intervals = (int)(max/delta);

	//LAB2
	int count_rec = 0; //conta o nº de recetores ocupados
	int count_qeue = 0; //conta o nº de chamadas na fila
	
	list  * qeue_list; //lista com as chamadas na fila
	qeue_list = NULL;
	int qeue_type; 
	double qeue_time = 0;

	double c_d = 0;

	// histogram array:
	int *histogram = malloc(intervals * sizeof(int));
	for (int k = 0; k < intervals; k++) histogram[k] = 0;

	int sample = 0;
	srand(time(NULL));

	while (sample < MAX_samp){
		//GERO EVENTO DE CHEGADA
		double u_a = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		double c_a = -log(u_a)/LAMBDA;

		//RETIRA O EVENTO DA LISTA
		if (event_list != NULL && c_d <= c_t) {
			ev_type = event_list -> type;
			ev_time = event_list -> time;
			event_list = __remove(event_list);
			count_rec--;
		}

		int i = (int)(c_a/delta);
		   
    	if (i >= intervals) i = intervals -1;

    	histogram[i]++;

		//Verifica se há receivers disponiveis
		if(count_rec < receivers){
			//SE HOUVER RECETORES LIVRES GERO EVENTO DE PARTIDA
			double u_d = (double)rand() / (double)((unsigned)RAND_MAX + 1);
			c_d = -log(u_d)/LAMBDA;
			count_rec++;

			if(count_qeue==0){
				event_list = __add(event_list, CHEGADA, c_a + ev_time); //adiciona o evento à lista
			}
			else{
				//RETIRA O EVENTO DA LISTA
				if (qeue_list != NULL) {
					qeue_type = qeue_list -> type;
					qeue_time = qeue_list -> time;
					qeue_list = __remove(qeue_list);
					count_qeue--;
				}
				event_list = __add(event_list, CHEGADA, c_a + ev_time); //adiciona o evento à lista
			}
		}
		else{
			//Verifica se há lugares na fila disponiveis
			if(count_qeue < qeue){
				count_qeue++;
				qeue_list = __add(qeue_list, CHEGADA, c_t + c_a);
			}
			else{
				block++;
			}
		}

		c_t = c_t + c_a;
		sample++;
	}

	double E_c = c_t/MAX_samp;
    printf("E[c] = %f\n", E_c);
	double E_b = block/MAX_samp;
	printf("E[b] = %f\n", E_b);
    
    for (int j = 0; j < intervals; j++) {
        printf("histogram[%d] = %d\n", j, histogram[j]);
    }
}
