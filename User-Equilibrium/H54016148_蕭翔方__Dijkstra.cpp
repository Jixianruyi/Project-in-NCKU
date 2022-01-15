#include <stdio.h>
#include <stdlib.h>
#define N 65535
#define M 2147483647
int main(void)
{
	int node_num[N],i = 1, j = 0, k, m, n, stack[N];
	struct ArcArray {int h, t, d;} arc[N];
	FILE *fp;
	fp = fopen("Philadelphia.txt", "r");
	while(fscanf(fp,"%d%d%d", &arc[i].h, &arc[i].t, &arc[i].d) != EOF) {
		if(arc[i].h != j) {
			node_num[arc[i].h] = i;
			j = arc[i].h;
		}
		i++;
	}
	j = j + 1;
	node_num[j] = i;
	printf("Enter origin and destination: ");
	scanf("%d%d", &m, &n);
	bool visit[j];
	for(i = 1; i < j; i++) {
		visit[i] = false;
	}
	int distance[j];
	for(i = 1; i < j; i++) {
		distance[i] = M;
	}
	distance[m] = 0;
	int predecessor[j];
	for (i = 1; i < j; i++) {
		predecessor[i] = 0;
	}
	/*printf("     ");
	for (i = 1; i < j; i++) {
		printf("%4d", i);
	}
	printf("\n d(i)");
	for (i = 1; i < j; i++) {
		if (distance[i] == M)
			printf(" INF");
		else
			printf("%4d", distance[i]);
	}
	printf("\n p(i)");
	for (i = 1; i < j; i++) {
		printf("%4d", predecessor[i]);
	}
	printf("\n\n");*/ 
	for (k = 1; k < j; k++) {
        int a = -1, min = M;
        for (i = 1; i < j; i++) {
            if (!visit[i] && distance[i] < min) {
                a = i;
                min = distance[i];
            }
        }
        if (a == -1)
			break;
        visit[a] = true;
        if(a == n)
        	break;
        for (i = node_num[a]; arc[i].h == a; i++) {
            int t = arc[i].t, d = arc[i].d;
            if (!visit[t] && distance[a] + d < distance[t]) {
                distance[t] = distance[a] + d;
                predecessor[t] = a;
            }
        }
        /*printf(" d(i)");
        for (i = 1; i < j; i++) {
        	if(distance[i] == M)
        		printf(" INF");
        	else
        		printf("%4d", distance[i]);
		}
		printf("\n p(i)");
		for (i = 1; i < j; i++) {
			printf("%4d", predecessor[i]);
		}
		printf("\n\n");*/ 
    }
    printf("\n%d\n", distance[n]);
    stack[1] = n;
    i = 2;
    while(n != m) {
    	stack[i] = predecessor[n];
    	n = predecessor[n];
    	i++;
	}
	for(i = i - 1; i > 0; i--) {
		printf("%d, ", stack[i]);
	}
	system("pause");
    return 0;
 }
