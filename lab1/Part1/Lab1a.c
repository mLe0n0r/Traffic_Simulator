#include<stdio.h>
#include<stdlib.h>
#include<math.h> //para usar a função random
#include<time.h>
#include "linked-list.h"

#define CHEGADA 0 //tipo de evento (no struct)
#define PARTIDA 1

#define lambda 5 //taxa de chegada
#define MAX_samp 100 //nº de samples

int main(void)
{
	list  * event_list;
	int ev_type; 
	double ev_time = 0;
	event_list = NULL;
	
	double c_t = 0; //para calcular a média de c
	double max = 5.0/lambda; //nº max de valores representaveis 
	double delta = 1.0/(5.0*lambda); 
	int intervals = (int)(max/delta);

	// histogram array:
	int *histogram = malloc(intervals * sizeof(int));
	for (int k = 0; k <= intervals; k++) histogram[k] = 0;

	int sample = 0;
	srand(time(NULL));

	while (sample < MAX_samp){
		double u = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		double c = -log(u)/lambda;

		//RETIRA O EVENTO DA LISTA
		if (event_list != NULL) {
			ev_type = event_list -> type;
			ev_time = event_list -> time;
			event_list = __remove(event_list);
		}

		int i = (int)(c/delta);   
    	if (i >= intervals) i = intervals;

    	histogram[i]++;

		event_list = __add(event_list, CHEGADA, c + ev_time); //adiciona o evento à lista

		c_t = c_t + c;
		sample++;
	}

	double E_c = c_t/MAX_samp;
    printf("E[c] = %f\n", E_c);
    
    for (int j = 0; j <= intervals; j++) {
        printf("histogram[%d] = %d\n", j, histogram[j]);
    }
}