#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 65535
#define M 2147483647
#define P 256

struct NetworkArray {int from, to; double length, alpha, beta, capacity, speed, tff, ta;} arc[N];
int node_num[N], node_count, arc_count, predecessor[N], mapping[500][500];
double x[N], y[N];

void dijkstra(int origin)
{
	bool visit[node_count];
	int i;
	
	for(i = 1; i < node_count; i++) {
		visit[i] = false;
	}
	
	double time[node_count];
	
	for(i = 1; i < node_count; i++) {
		time[i] = M;
	}
	time[origin] = 0;
	
	for (i = 1; i < node_count; i++) {
		predecessor[i] = 0;
	}

	for (int k = 1; k < node_count; k++) {
        int a = -1;
		double min = M;
		
        for (i = 1; i < node_count; i++) {
            if (!visit[i] && time[i] < min){
                a = i;
                min = time[i];
            }
        }
        if (a == -1)
			break;
			
        visit[a] = true;
        
        for (i = node_num[a]; arc[i].from == a; i++) {
            int to = arc[i].to;
			double ta = arc[i].ta;
			
            if (!visit[to] && time[a] + ta < time[to]){
                time[to] = time[a] + ta;
                predecessor[to] = a;
            }
        }
    }
}

double ditonic(double alpha)
{
	double sum = 0, x_plus;
	
	for(int i = 1; i < arc_count; i++){
		x_plus = x[i] + alpha * (y[i] - x[i]);
		sum += arc[i].tff * x_plus + arc[i].alpha * arc[i].tff / (arc[i].beta + 1) * arc[i].capacity * pow((x_plus / arc[i].capacity), (arc[i].beta + 1));
	}
    return sum;
}

double golden_section(void)
{
    double lower_bound = 0, upper_bound = 1, left_interior, right_interior;
    
	while(upper_bound - lower_bound >= 0.000001){
        left_interior = upper_bound - 0.618 * (upper_bound - lower_bound);
        right_interior = 0.618 * (upper_bound - lower_bound) + lower_bound;
        
		if(ditonic(left_interior) > ditonic(right_interior))
            lower_bound = left_interior;
        else
            upper_bound = right_interior;
    }
    
	return ((upper_bound + lower_bound) / 2);
}
int main(void)
{	//read in OD data file 
	FILE *od_file;	
	od_file = fopen("Austin OD.txt", "r");
	
	int od_count = 1;
	double temp[N];
	//od_count: as a counter of the number of data read, where data are each stored in temp[]
	while(fscanf(od_file,"%lf", &temp[od_count]) != EOF){
		od_count++;
	}
	fclose(od_file);
	//total: the total number of nodes
	int total = (int)sqrt(od_count - 1), i, j;
	double od[P][P];
	// let od[][] be the trip rate of each od pair. Retrieve the data stored in temp[] one by one
	for(i = 1; i <= total; i++){
		for(j = 1; j <= total; j++){
			od[i][j] = temp[(i - 1) * total + j];
		}
	}
	//read in Network file
	FILE *network;
	network = fopen("Austin Network.txt", "r");
	//if inside the data file there are characters up ahead, line 111 and 112 should be utilized to get rid of it in order to correctly read data in 
	/*char whatever[P];
	fgets(whatever, P, network);*/
	node_count = 0, arc_count = 1;
	//read in and stored the data, notice that # of lanes were not used in this program so simply replace it over and over in temp[0]
	while(fscanf(network,"%d%d%lf%d%lf%lf%lf%lf", &arc[arc_count].from, &arc[arc_count].to, &arc[arc_count].length, &temp[0], &arc[arc_count].alpha, &arc[arc_count].beta, &arc[arc_count].capacity, &arc[arc_count].speed) != EOF){
		mapping[arc[arc_count].from][arc[arc_count].to] = arc_count;
		//mapping[][]: provides a mapping between an od pair and # of arc it represents
		if(arc[arc_count].from != node_count){
			node_num[arc[arc_count].from] = arc_count;
			node_count = arc[arc_count].from;
		}
		arc_count++;
	}
	fclose(network);
	node_count = node_count + 1;
	node_num[node_count] = arc_count;
	//node_num[]: provides an index of where one can find a specific node as an origin in the "network" file
	for(i = 1; i < arc_count; i++){
		arc[i].tff = arc[i].length / arc[i].speed;
		arc[i].ta = arc[i].tff;
	}
	//calculates free flow travel time
	for(i = 1; i < arc_count; i++){
		x[i] = 0;
	}
	//after searching each origin to every destination's shortest path through dijkstra, perform all-or-nothing into all of that path's link, then go on to another origin
	for(i = 1; i <= total; i++){
		dijkstra(i);
		for(int k = 1; k <= total; k++){
			int des_now = k;
			while(des_now != i){
				x[mapping[predecessor[des_now]][des_now]] += od[i][k];
				des_now = predecessor[des_now];
			}
		}
	}

	double x_plus[arc_count];
	
	while(1){
		for(i = 1; i < arc_count; i++){
			arc[i].ta = arc[i].tff * (1 + arc[i].alpha * (pow(x[i] / arc[i].capacity, arc[i].beta)));
		}
		//update link travel time
		for(i = 1; i < arc_count; i++){
			y[i] = 0;
		}
		
		for(i = 1; i <= total; i++){
			dijkstra(i);
			for(int k = 1; k <= total; k++){
				int des_now = k;
				while(des_now != i){
					y[mapping[predecessor[des_now]][des_now]] += od[i][k];
					des_now = predecessor[des_now];
				}
			}
		}
		//obtain appropiate alpha through golden section search
		double alpha = golden_section();
		//calculates the new trip rate on each link
		for(i = 1; i < arc_count; i++){
			x_plus[i] = x[i] + alpha * (y[i] - x[i]);
		}
	
		double numerator = 0, denominator = 0;
		//convergence test
		for(i = 1; i < arc_count; i++){
			numerator += (x_plus[i] - x[i]) * (x_plus[i] - x[i]);
			denominator += x[i];
		}
		//line 183 and 184 could be utilized for the examination of convergence progress
		/*printf("%lf\n%lf\n", sqrt(numerator), denominator);
		system("pause");*/
		if(sqrt(numerator) / denominator < 0.000001){
			break;
		}
		else{
			for(i = 1; i < arc_count; i++){
				x[i] = x_plus[i];
			}
		}
	}
	//output the result into a file
	FILE *result;
	result = fopen("User Equilibrium.txt", "w");
	for(i = 1; i < arc_count; i++){
		fprintf(result, "From: %d ; To: %d ; Flow: %lf\n", arc[i].from, arc[i].to, x_plus[i]);
	}
	system("pause");
	return 0;
}
