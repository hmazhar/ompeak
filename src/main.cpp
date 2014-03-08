#include "omp.h"
#include <stdio.h>
#include <string>
#include <math.h>
#include <nmmintrin.h>
using namespace std;
class __attribute__ ((aligned(16))) float4 {
public:
	union {
		struct {float x, y, z, w;};
		__m128 mmvalue;
	};
	inline float4() 		:mmvalue(_mm_setzero_ps()) { }
	inline float4(float a) 	:mmvalue(_mm_set1_ps(a)) {}
	inline float4(__m128 m) :mmvalue(m) {}
	inline float4 operator+(const float4& b) const {	return _mm_add_ps(mmvalue, b.mmvalue);}
};

int main(int argc, char *argv[]) {
	int thread_num = 1;
	bool single_test = false;
	if (argc > 1) {	thread_num = atoi(argv[1]);}
	if (argc > 2 && string(argv[2])=="-s") {single_test = true; printf("Performing constant thread test\n");}
	double runs = 28;
	float4* A = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );
	float4* B = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );
	float4* C = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );
	float4* D = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );

	//Generate data
	#pragma omp parallel for 
	for (int i = 0; i < 134217728; i++) {
		A[i] = float4(i);
		B[i] = float4(1.0/float(i));
		C[i] = float4(2.0/float(i));
		D[i] = float4(3.0/float(i));
	}
	printf("Memory transfered in MBytes (column) for a certain number of threads (rows): \n");
	printf("   \t");
	for (int i = 14; i < runs; i++) {
		printf(" %4.2f\t",sizeof(float4)*pow(2,i)/1024.0/1024.0);
	}
	printf("\n");

	int start_threads = 1;
	int max_threads = thread_num;

	if(single_test){
		start_threads = thread_num;
	}

	for (int threads = start_threads; threads <= max_threads; threads++) {
		omp_set_num_threads(threads);
		printf("%3d\t", threads);
	for (int i = 14; i < runs; i++) {
		unsigned int MAX_ITEMS  = pow(2,i);
		//Clear the cache!
		#pragma omp parallel for 
		for (unsigned int i = 0; i < 134217728; i+=4) {
			C[i] = D[i]+C[i];
		}
		//Run benchmark
		double start = omp_get_wtime();
		#pragma omp parallel for
		for (int id = 0; id < MAX_ITEMS; id++) {
			A[id+0]= A[id+0]+B[id+0];
			A[id+1]= A[id+1]+B[id+1];
			A[id+2]= A[id+2]+B[id+2];
			A[id+3]= A[id+3]+B[id+3];
		}
		double end = omp_get_wtime();
		printf(" %0.3f\t",(3 * 4 * 4) * MAX_ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0);
	}
	printf("\n");
}
	_mm_free(A);
	_mm_free(B);
	_mm_free(C);
	_mm_free(D);
	return 0;
}
