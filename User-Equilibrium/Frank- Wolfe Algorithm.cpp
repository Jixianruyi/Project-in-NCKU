/* so called the User Equilibrium program */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define Total_node_number 24
#define Infinity 2147483647
#define M 65535

int link, shortest_path[M] = {0}, from_node[M] = {0}, To_node[M] = {0}, pointer[M] = {0}, OD[Total_node_number+1][Total_node_number+1];

double cost[M] = {0},tff[M] = {0}, capacity[M] = {0}, alpha,x_flow[M] = {0}, y_flow[M] = {0},xplus_flow[M] = {0}/***OD[Total_node_number+1][Total_node_number+1]***/; 

void Dijkstra(int start_node, int end_node);
double BPR_function(double tff, double xa, double ca);
void Direction_finding(void);
double Golden_section_search(void);
double Equation(double x);
double Convergence(void);


int main(void)
{
	int i, n, od_count = 1, k;
	double temp[M];	
	FILE *OD_table;
	OD_table = fopen("Sioux Falls OD.txt", "r");
	while(fscanf(OD_table,"%lf", &temp[od_count]) != EOF){
		od_count++;
	}
	fclose(OD_table);
	for(int i = 1; i <= Total_node_number; i++){
		for(int j = 1; j <= Total_node_number; j++){
			OD[i][j] = temp[(i - 1) * Total_node_number + j];
		}
	}
	
	//for (int i =1 ; i <= Total_node_number; i++){
	//	for(int j = 1; j <= Total_node_number; j++){
	//	printf("%d\n", OD[i][j]);	
	//	system("pause");
	//	}
	//}

	/*** construct OD table****/
	FILE *network;
	network = fopen("Sioux Falls Network.txt", "r");	
	for (i=1;fscanf(network,"%f",&n) == 1;i++);
	link = i/8;
	fclose(network);
	/*** count link number ****/
	FILE *network2;
	network2 = fopen("Sioux Falls Network.txt", "r");
	for (int i = 1; i <= link; i++){	
		fscanf(network2,"%d%d%lf%*d%*f%*d%lf%*d%", &from_node[i], &To_node[i], &cost[i], &capacity[i]);
	}
	/**** construct from_node, To_node, cost, capacity****/
	for (int i = 1; i <= link; i++){
		tff[i] = cost[i];
	}
	/*** to construct a tff for golden section ***/
	pointer[1] = 1;
	
	for(int i=2, n=1; i <=Total_node_number+1; i++){
		do {
			n++;
	 		pointer[i] = n;

		} while (from_node[n] == from_node[n-1]);
    }
	/*** construct pointer   ****/

	
	
		for (int start = 1; start <= Total_node_number ; start++){
			for( int end = 1 ; end <= Total_node_number; end++){
				//printf("%d %d", start, end);
				//system("pause");
				Dijkstra(start, end);
				for (int i = 1; i <= link ; i++ ){
					if (shortest_path[i]!= 0){
						for(int j = pointer[shortest_path[i]]; j <= pointer[shortest_path[i]+1] - 1; j++){
							if (To_node[j] == shortest_path[i+1]){
								//printf("%d",j);
								//system("pause");
								x_flow[j] = x_flow[j] + OD[start][end];
								//printf("%f", x_flow[j]);
								//system("pause");
							}
						}
					}	
				}
				for (int i = 1; i <= link; i++){
					shortest_path[i] = 0;
				}
			}
		}
		//for(int i = 1; i <= link ; i++){
		//	printf("%f\n", x_flow[i]);
	
		//}
		//system("pause");
		
		/*** step 0: Initialization   ****/
	while (1) {
		
		//for (int i = 1; i <= link ; i++){
		//	if (cost[i] != 0)
		//	printf("%f  ", cost[i]);
		//}
		//system("pause");
		
		for (int i = 1; i <= link ; i++){
			cost[i] = BPR_function(tff[i], x_flow[i],capacity[i]);
		}
		
		//for (int i = 1; i <= link ; i++){
		//	if (cost[i] != 0)
		//	printf("%lf  ", cost[i]);
		//}
		//system("pause");
		
		
		/*** step 1 Update   ****/
		
		Direction_finding();
		
		//for(int i = 1; i <= link; i++){
		//	
		//	printf("%lf   %lf\n", x_flow[i],y_flow[i]);
			
		//}
		//system("pause");
		/*** To obtain ya    *****/
		alpha = Golden_section_search();
		//printf("%.8f", alpha);
		//system("pause");
		/**** Find alpha   ****/
		for(int i= 1; i <= link ; i++){
			xplus_flow[i] = x_flow[i] + alpha * ( y_flow[i]- x_flow[i]);
		}	
		/*****  Move   *****/
		
		//for(int i = 1; i <= link; i++){
		//	
		//	printf("%f   %f  %.8f   %f\n",x_flow[i], y_flow[i], alpha ,xplus_flow[i]);			
		//}
		//system("pause");
		

		/*** to clean up y_flow & x_flow  ****/
		
		double converg = Convergence();
		
		for (int i = 1; i <= link; i++){
			y_flow[i] = 0;
			x_flow[i] = 0;
		}
		printf("Iteration: %d\n", k);
		k++;
		printf("%.6lf\n", converg);
		//system("pause");
		if (converg <  0.00001){

			break;
		}
		else
			for( int i = 1; i <= link; i++){
				x_flow[i] = xplus_flow[i];				
			}
		/*** Convergence test *****/	
	}
		
		
				
	FILE *result;
	result =fopen("User Equillibrium.txt", "w");	
	for (int i = 1; i <= link; i++){
		fprintf(result, "From %d To %d : %.8lf\n", from_node[i], To_node[i], xplus_flow[i]);	
	}
		
	return 0;
	} 




void Dijkstra(int start_node, int end_node)                                                                                                                
{

	int predecessor[M] = {0}, node_status[M] = {0}, bridge[M] = {0}; 
	
	double label[M] = {0}; 
	
	for (int i = 1; i <= Total_node_number; i++ ) {
		label[i] = Infinity;
		predecessor[i] = 0;
	}

	label[start_node] = 0;

	for (int k = 1; k <= Total_node_number; k++ ) {
		int next_node = -1;
		double min = Infinity;
		for (int i = 1; i <= Total_node_number; i++)  {
			if(label[i] < min && node_status[i] == 0) {
				next_node = i;
				min = label[i];	
			}		
		}
		
		if (next_node == end_node || next_node == -1) break;
		
		node_status[next_node]++;
		
		for (int i= pointer[next_node]; i <= (pointer[next_node+1]-1); i++ ) {
			if (label[next_node] + cost[i] < label[To_node[i]] ) {
				label[To_node[i]] = label[next_node] + cost[i];
				predecessor[To_node[i]] = next_node;
			}
		}		
	}
	
	int pre = 1, k,i; 
	k = end_node;
		
	for(int n = 1 ;pre != 0; n++) {
		bridge[n] = k;
		pre = predecessor[k];
		k = pre;	
	}
	
	for(int i = Total_node_number, j = 1; i >= 1 ; i--)
	{
		if (bridge[i] != 0){	
		shortest_path[j] = bridge[i];
		j++;
		}
	}
	
	
	//for (int i = 1; i <= link; i++){
	//	if (shortest_path[i] != 0)
	//	printf("%d  ", shortest_path[i]);
	//}
	//system("pause");
	
	
	/*** turn the order back ****/
	
	//for (int k = 1, j = 1; k <= Total_node_number; k++){
	//	if (shortest_path[k] != 0) {
	//		for (int i =pointer[shortest_path[k]]; i <= (pointer[shortest_path[k]+1]-1);i++){
	//			if(To_node[i] == shortest_path[k + 1]){
	//				shortest_path[j] = i;  	
	//				j++;				
	//			}
	//		}
	//	}
	//}

	
	/***  shortest path now contains links of shortest path instead of node   ****/	
}                                                     


double BPR_function(double tff, double xa, double ca)
{
	double ta;
	ta = tff*(1+0.15*(xa/ca)*(xa/ca)*(xa/ca)*(xa/ca));
	return ta;	
}



void Direction_finding(void)
{
	for (int start = 1; start <= Total_node_number ; start++){
		for( int end = 1 ; end <= Total_node_number; end++){
		   		//printf("%d %d", start, end);
				//system("pause");
				Dijkstra(start, end);
				for (int i = 1; i <= link ; i++ ){
					if (shortest_path[i]!= 0){
						for(int j = pointer[shortest_path[i]]; j <= pointer[shortest_path[i]+1] - 1; j++){
							if (To_node[j] == shortest_path[i+1]){
								//printf("%d",j);
								//system("pause");
								y_flow[j] = y_flow[j] + OD[start][end];
								//printf("%f", x_flow[j]);
								//system("pause");
							}
						}
					}	
				}
			
				for (int i = 1; i <= link; i++){
					shortest_path[i] = 0;
				}	
		}
	}
}			
	


double Golden_section_search(void)
{
	double y1, y2, x1 = 0.0f , x2 = 1.0f, x1p, x2p;
	
	while(x2-x1>=0.0000001f) {
		
		x1p = (x1 + (x2-x1)*0.382f);
		x2p = (x2 - (x2-x1)*0.382f);	
		y1 = Equation(x1p);
		y2 = Equation(x2p);

		if (y1>y2)
			x1 = x1p;
		else
			x2  = x2p;
	}
	return ((x1+x2)/2);
}

double Equation(double x)
{
	double sum = 0, xnp1_flow = 0;
	for (int i = 1; i <= link; i++){
		xnp1_flow = x_flow[i] + x *(y_flow[i] - x_flow[i]);	
		sum = sum + (tff[i]*xnp1_flow + 0.03*tff[i] * capacity[i] * (xnp1_flow /capacity[i])* (xnp1_flow /capacity[i])* (xnp1_flow /capacity[i])* (xnp1_flow /capacity[i])* (xnp1_flow /capacity[i]));
		//printf("%lf", sum)
		//system("pause");
	}
	
	return sum;
}
   
double Convergence(void)
{
	double up = 0, down = 0;
	for (int i = 1; i <= link ; i++) {
		up = up + (xplus_flow[i] - x_flow[i]) * (xplus_flow[i] - x_flow[i]);
		down = down + x_flow[i];
	}
	return (sqrt(up) / down);	
}



