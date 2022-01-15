#include<stdio.h>
int main(void)
{
	// the equation being y = 3x^2-12x+3
	// x1p = x1', x2p = x2'
	double y1, y2, x1 = -1000.0f , x2 = 1000.0f, x1p, x2p;
	
	while(x2-x1>=0.0000001f) {
		
		x1p = (x1 + (x2-x1)*0.382f);
		x2p = (x2 - (x2-x1)*0.382f);		
		y1 = 3 * x1p * x1p - 12 * x1p +3;
		y2 = 3 * x2p * x2p - 12 * x2p +3;
		if (y1>y2)
			x1 = x1p;
		else
			x2  = x2p;
	}
	printf("%f", (x1+x2)/2);
	return 0;
}
