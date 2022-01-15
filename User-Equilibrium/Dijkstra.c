/* Dijkstra Algo*/
/* remember to change the Total_node_number */
/**** remember to add-in the exit point if can't find shortest path  ****/
#include<stdio.h>
#include<stdlib.h>
#define Total_node_number 13389
#define Infinity 2147483647
int main(void)
{
	FILE *fp;
	int i,n, link, start_node, end_node, next_node;
	fp = fopen ("phila_network.txt", "r");
	/** counting link number **/
	for (i=1;fscanf(fp,"%d",&n) == 1;i++);
	/** cosidering put something in here but later ***/	
	link = i/3;
	/*** end of link number counting ***/
	int from_node[link+1], To_node[link+1], cost[link+1], pointer[Total_node_number+2], predecessor[Total_node_number+1], 
	node_status[Total_node_number+1], label[Total_node_number+1];  
	/**** all the data should be stored in the array begin from a[1], +2 in pointer because of the dummy node*****/
	fp = fopen ("phila_network.txt", "r");
	for (int i = 1; i <= link; i++){
		fscanf(fp,"%d%d%d", &from_node[i], &To_node[i], &cost[i]);	
	}
	/**** read network file and store them into three different array  ******/
	pointer[1] = 1;
	for (int i=2, n=1; i <=Total_node_number+1;  i++)  {
		do {
			n++;
	  		pointer[i] = n;
		} while (from_node[n] == from_node[n-1]);
    }
	/**** constructing the pointer array,    ******/
 	printf("Insert an node you plan to start: ");
 	scanf("%d",&start_node);
 	printf("Insert an node you plan to end: ");
 	scanf("%d", &end_node);
	for (int i = 1; i <= Total_node_number; i++ ) {
		label[i] = Infinity;
		predecessor[i] = 0;
	}
	/*** set all node to infinity and predecessro all 0  ******/
	label[start_node] = 0;
	while (1) {
		int min = Infinity;
		for (int i= pointer[start_node]; i <= (pointer[start_node+1]-1); i++ ) {
			if (label[start_node] + cost[i] < label[To_node[i]] ) {
				label[To_node[i]] = label[start_node] + cost[i];
				predecessor[To_node[i]] = start_node;
			}
		}
		node_status[start_node]++;
	/*** to marke the status of node, marked by 1 unmarked by 0  ****/		
		for (int i = 1; i <= Total_node_number;i++)  {
			if(label[i] < min && node_status[i] == 0) {
				min = label[i];
				next_node = i;
			}		
		}
		if (label[end_node] ==  min && end_node == next_node) {
			/**** got a question here ****/
			printf("The shortest path is:");   
			int shortest_path[Total_node_number+1] = {0}, pre = 1, i; 
			i = end_node;
			/*** put the value of end_node into i   ***/
			for  (int n = 1 ;pre != 0; n++) {
				shortest_path[n] = i;
				pre = predecessor[i];
				i = pre;	
			}
			for (int i = Total_node_number; i >= 1 ; i--) {
				if (shortest_path[i] != 0)				
				printf(" %d", shortest_path[i]);
			}                                                     
			printf("\nCost: %d", label[end_node]);
			exit(EXIT_SUCCESS);		
		}
		start_node = next_node;		
	}
	return 0;    
}
