#include<stdio.h>
#include<stdlib.h>
#include<math.h> 
#include<time.h>
#include "linked-list.h"

#define CHEGADA 0 
#define PARTIDA 1

#define lambda 5 //taxa de chegada
#define MAX_samp 100 //nº de samples

int main(void){
    list  * event_list;
	int ev_type; 
	double ev_time = 0.0;
	event_list = NULL;

    double delta_inf = 0.001;
    double present_time = 0.0;
    double c_total = 0;

    double max = 5.0/lambda; //nº max de valores representaveis 
	double delta = 1.0/(5.0*lambda); 
	int intervals = (int)(max/delta);

    int sample = 0;
    double p = delta_inf * lambda;

    srand(time(NULL));

    // histogram array:
	int *histogram = malloc(intervals * sizeof(int));
	for (int k = 0; k <= intervals; k++) histogram[k] = 0;

    while (sample < MAX_samp) {
        double prob = (double)rand() / (double)((unsigned)RAND_MAX + 1);
        present_time += delta_inf;

        if (prob < p){ // houve chegada
            if (event_list != NULL) {
                ev_type = event_list -> type;
                ev_time = event_list -> time;
                event_list = __remove(event_list);
            }

            double c = present_time - ev_time;

            int i = (int)(c/delta);   
            if (i >= intervals) i = intervals;

            histogram[i]++;
            event_list = __add(event_list, CHEGADA, present_time);

            c_total += c;
            sample++;
        }

    }

    double E_c = c_total/MAX_samp;
    printf("E[c] = %f\n", E_c);

    for (int j = 0; j <= intervals; j++) {
        printf("histogram[%d] = %d\n", j, histogram[j]);
    }
}
