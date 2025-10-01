#include<stdio.h>
#include<stdlib.h>
#include "linked-list.h"

// Function that removes the first element of the linked list
list * __remove (list * pointer)
{
	list * lp = (list *)pointer -> next;
	free(pointer);
	return lp;
}

// Function that adds a new element to the list, sorting the list in chronological order
list * __add (list * pointer, int n_type, double n_time)
{
	list * lp = pointer;
	list * p_aux, * p_next;
	if(pointer == NULL)
	{
		pointer = (list *) malloc(sizeof (list));
		pointer -> next = NULL;
		pointer -> type = n_type;
		pointer -> type = n_type;
		return pointer;
	}
	else
	{
		if (pointer->time > n_time) {
	        p_aux = (list *) malloc(sizeof (list));
	        p_aux -> type = n_type;
            p_aux -> time = n_time;
            p_aux -> next = (struct list *) pointer;
            return p_aux;
	    }

		p_next = (list *)pointer -> next;
		
		while(pointer != NULL)
		{
			if((p_next == NULL) || ((p_next -> time) > n_time))
				break;
			pointer = (list *)pointer -> next;
			p_next = (list *)pointer -> next;
		}
		p_aux = (list *)pointer -> next;
		pointer -> next = (struct list *) malloc(sizeof (list));
		pointer = (list *)pointer -> next;
		if(p_aux != NULL)
			pointer -> next = (struct list *)p_aux;
		else
			pointer -> next = NULL;
		pointer -> type = n_type;
		pointer -> time = n_time;
		return lp;
	}
}

// Function that prints in the screen all the element of the linked list
void __print (list * pointer)
{
	if(pointer == NULL)
		printf("List empty!\n");
	else
	{
		while(pointer != NULL)
		{
			printf("Type=%d\tTime=%lf\n", pointer -> type, pointer -> time);
			pointer = (list *)pointer -> next;
		}
	}
}

// A simple example of using the functions defined above
// int main(void)
// {
// 	list  * event_list;
// 	int ev_type; double ev_time;
// 	event_list = NULL;
// 	event_list = __add(event_list, 1, 0.6);
// 	event_list = __add(event_list, 0, 0.4);
// 	event_list = __add(event_list, 1, 0.3);
// 	event_list = __add(event_list, 2, 0.5);
// 	event_list = __add(event_list, 1, 0.5);
// 	event_list = __add(event_list, 0, 0.2);
// 	event_list = __add(event_list, 1, 0.1);
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if (event_list != NULL) {
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
	
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if (event_list != NULL) {
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
	
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if (event_list != NULL) {
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
	
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if ( event_list != NULL) {
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if ( event_list != NULL)
// 	{
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if ( event_list != NULL)
// 	{
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
	
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if ( event_list != NULL)
// 	{
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
	
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if ( event_list != NULL)
// 	{
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
	
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	if ( event_list != NULL)
// 	{
// 		ev_type = event_list -> type;
// 		ev_time = event_list -> time;
// 		event_list = __remove(event_list);
// 		printf("\nEVENT REMOVED\n");
// 		printf("Type=%d\tTime=%lf\n", ev_type, ev_time);
// 	}
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);

// 	event_list = __add(event_list, 2, 0.5);
// 	event_list = __add(event_list, 1, 0.3);
// 	printf("\nCURRENT LIST\n");
// 	__print(event_list);
// }

