#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

//gcc -Wall -O2 matmul-normal.c -o matmul-normal -DN=400

void get_walltime(double *wtc){
	struct timeval tp;
	gettimeofday(&tp,NULL);
	*wtc = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main(){

	float *a, *b, *c;
	double ts, te, mega;

	float *pa, *pb, *pc;
	float sum;
	
	int i, j, k;

//Array allocation

	a = (float *)malloc(N*N*sizeof(float)); 
	if (a==NULL){
		printf("error array a!\n");
		exit(1);
	}

	b = (float *)malloc(N*N*sizeof(float)); 
	if (b==NULL){
		printf("error array b!\n");
		free(a);
		exit(1);
	}

	c = (float *)malloc(N*N*sizeof(float)); 
	if (c==NULL){
		printf("error array c!\n");
		free(a);
		free(b);
		exit(1);
	}

//Warmup

	for(i=0; i<N*N; i++){
		a[i] = 2.0;
		b[i] = 3.0;
		c[i] = 20.0;
	}

//Start time

	get_walltime(&ts);

//Workload

	pc=c;

	for(k=0; k<N; k++){
		pb = b;
		for(j=0; j<N; j++){
			pa = a + k*N;
			sum = 0;
			for(i=0; i<N; i++){
				sum += (*pa) * (*pb);
				pa++;
				pb++;
			}
			*pc = sum;
			pc++;
		}
	}

//End time

	get_walltime(&te);

//Check

	for(i= 0; i<N;i++){
		for(j=0;j<N;j++){
			if(c[j*N+i] != 6*N){
				printf("Error in result");
				exit(3);
			}
		}
	}

//Print time
	
	mega = (2.0*N*N)/((te-ts)*1e6);
	
	printf("Maccesses/sec = %f\n", mega);

//Free

	free(a);
	free(b);
	free(c);
	
	return 0;
}