#include<stdio.h>
#include<stdlib.h>

double max(double x);

int main(void)
{
	// the equation being y = 3x^2-12x+3
	// x1p = x1', x2p = x2'
	double y1, y2, x1 = 0.0f , x2 = 8.0f, x1p, x2p;
	
	while(x2-x1>=0.0000001f) {
		
		x1p = (x1 + (x2-x1)*0.382f);
		x2p = (x2 - (x2-x1)*0.382f);	
		y1 = max(x1p);
		y2 = max(x2p);

		if (y1>y2)
			x1 = x1p;
		else
			x2  = x2p;
	}
	printf("%f", (x1+x2)/2);
	return 0;
}


double max(double x)
{
	double y1, y2, y3;
	y1 = x*x - 6*x + 9;
	y2 = 2- x/3;
	y3 = x/5;
	if (y1 > y2) {
	
		if (y1 > y3)
			return y1;
		else
			return y3;
	}
	else {
		if (y2 > y3)
			return y2;
		else
			return y3;
	}
}
