#include<stdio.h>
#include<stdlib.h>
#include<math.h> //para usar a função random
#include<time.h>
#include "linked-list.h"

#define CHEGADA 0 //tipo de evento (no struct)
#define PARTIDA 1

#define lambda 5 //taxa de chegada
#define MAX_samp 100 //nº de samples

#define receivers 2 //nº de recetores
#define qeue 5 //tamanho da fila

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

	int count_rec = 0; //conta o nº de recetores ocupados
	int count_qeue = 0; //conta o nº de chamadas na fila
	int block = 0; //conta o nº de chamadas bloqueadas

	// histogram array:
	int *histogram = malloc(intervals * sizeof(int));
	for (int k = 0; k < intervals; k++) histogram[k] = 0;

	int sample = 0;
	srand(time(NULL));

	while (sample < MAX_samp){
		double u_a = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		double c_a = -log(u)/lambda;

		double u_d = (double)rand() / (double)((unsigned)RAND_MAX + 1);
		double c_d = -log(u)/lambda;

		//Verifica se há receivers disponiveis
		if(count_rec < receivers){

			count_rec++;
		}
		else{
			//Verifica se há lugares na fila disponiveis
			if(count_qeue < qeue){

				count_qeue++;
			}
			else{

				block++;
			}
		}

		//RETIRA O EVENTO DA LISTA
		if (event_list != NULL) {
			ev_type = event_list -> type;
			ev_time = event_list -> time;
			event_list = __remove(event_list);
		}

		int i = (int)(c/delta);
		   
    	if (i >= intervals) i = intervals -1;

    	histogram[i]++;

		event_list = __add(event_list, CHEGADA, c + ev_time); //adiciona o evento à lista

		c_t = c_t + c;
		sample++;
	}

	double E_c = c_t/MAX_samp;
    printf("E[c] = %f\n", E_c);
    
    for (int j = 0; j < intervals; j++) {
        printf("histogram[%d] = %d\n", j, histogram[j]);
    }
}