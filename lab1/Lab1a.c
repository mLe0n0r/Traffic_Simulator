#include<stdio.h>
#include<stdlib.h>
#include<math.h> //para usar a função random
#include<time.h>
#include "linked-list.h"

#define CHEGADA 0 //tipo de evento (no struct)
#define PARTIDA 1

#define lamda 5 //taxa de chegada
// #define RAND_MAX 1 
#define MAX_s 50 //nº de samples


int main(void)
{
	list  * event_list;
	int ev_type; 
	double ev_time = 0;
	event_list = NULL;
	
	double c_t = 0; //para calcular a média de c
	double max = 5.0/lamda; //nº max de valores representaveis 
	double delta = 1.0/(5.0*lamda); 
	int intervals = (int)(max/delta);

	// histogram array:
	int *histogram = malloc(intervals * sizeof(int));
	for (int k = 0; k < intervals; k++) histogram[k] = 0;

	int sample = 0;
	//srand(time(NULL));

	while (sample < MAX_s){
		sample++;
		double c = -log((double)rand() / (double)((unsigned)RAND_MAX + 1))/lamda;

		//RETIRA O EVENTO DA LISTA
		if (event_list != NULL) {
			ev_type = event_list -> type;
			ev_time = event_list -> time;
			event_list = __remove(event_list);
		}

		int i = (int)(c / delta);      
    	if (i >= intervals-1) i = intervals-1;
    	histogram[i]++;

		event_list = __add(event_list, CHEGADA, c+ev_time); //adiciona o evento à lista

		c_t = c_t + c;
	}

	double E_c = c_t/MAX_s;
    printf("E[c] = %f\n", E_c);
    
    for (int j = 0; j < intervals + 1; j++) {
        printf("histogram[%d] = %d\n", j, histogram[j]);
    }
}