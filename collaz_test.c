#include<stdio.h>

unsigned int collaz(unsigned int);

int main() {

	unsigned int i = 10;	

	while (1) {
		printf("i = %d\n", i);
		
		if (i == 1) break;

		i = collaz(i);
	} 
}