#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pmmintrin.h>

//gcc -Wall -O2 matmul-sse.c -o matmul-sse -DN=400

void get_walltime(double *wtc){
	struct timeval tp;
	gettimeofday(&tp,NULL);
	*wtc = (double)(tp.tv_sec+tp.tv_usec/1000000.0);
}

int main(){

	int i, j, k;

	float *a, *b, *c, *zero;
	float *pa = a, *pb = b, *pc = c;
	double ts, te, mega;

	__m128 *va, *vb;

	
//Array allocation
	
	i=0;
	i = posix_memalign((void**)&a, 16, N*N*sizeof(float));
	if(a!=0) {
		printf("error array a!\n");
		exit(1);
	}
	j = posix_memalign((void**)&b, 16, N*N*sizeof(float));
	if(b!=0) {
		printf("error array b!\n");
		free(a);
		exit(1);
	}
	k = posix_memalign((void**)&c, 16, N*N*sizeof(float));
	if(c!=0) {
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

	for (i=0; i<4; i++){
		zero[i] = 0.0;
	}

	__m128 *sum = (__m128*) zero;
	__m128 tmp;

//Start time

	get_walltime(&ts);

//Workload
	

	for( k = 0; k < N; k++){	
		vb =(__m128*) pb;
		for(j = 0; j < N; j++){
			pa = a + k*N;
			va = (__m128*) pa;
			*sum = _mm_set_ps(0, 0, 0, 0);

			for( i = 0; i < N; i+=4){   
				*sum = _mm_add_ps(*sum, _mm_mul_ps(*va, *vb));
				va++, vb++;
			}

			tmp =_mm_hadd_ps(*sum, *sum);
			*sum =_mm_hadd_ps(tmp, tmp);
			*pc = zero[0];
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