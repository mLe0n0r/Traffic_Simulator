#include<stdio.h>
#include<stdlib.h>
#include<math.h> //para usar a função random
#include<time.h>
#include "linked-list.h"

#define CHEGADA 0 //tipo de evento (no struct)
#define PARTIDA 1

#define lambda 5 //taxa de chegada
#define MAX_samp 100 //nº de samples

#define receivers 1 //nº de recetores
#define qeue 1 //tamanho da fila

int main(void)
{
	list  * event_list;
	int ev_type; 
	double ev_time = 0;
	event_list = NULL;
	
	//LAB1
	double c_t = 0; //para calcular a média de c
	double max = 5.0/lambda; //nº max de valores representaveis 
	double delta = 1.0/(5.0*lambda); 
	int intervals = (int)(max/delta);

	//LAB2
	int count_rec = 0; //conta o nº de recetores ocupados
	int count_qeue = 0; //conta o nº de chamadas na fila
	int block = 0; //conta o nº de chamadas bloqueadas
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
		double c_a = -log(u_a)/lambda;

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
			c_d = -log(u_d)/lambda;
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
    
    for (int j = 0; j < intervals; j++) {
        printf("histogram[%d] = %d\n", j, histogram[j]);
    }
}
