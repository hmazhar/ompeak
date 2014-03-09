#include "omp.h"
#include <stdio.h>
#include <string>
#include <math.h>
#include <unistd.h>
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


void ClearCache(float4* C, float4* D, unsigned int max){


		#pragma omp parallel for 
		for (unsigned int i = 0; i < max; i+=4) {
			C[i] = D[i]+C[i];
		}

}

void MemoryTest_CacheLine(unsigned int i, float4* A, float4* B){
		unsigned int ITEMS  = pow(2,i);
		//Clear the cache!

		//Run benchmark
		double start = omp_get_wtime();
		#pragma omp parallel for
		for (unsigned int id = 0; id < ITEMS; id+=4) {
			A[id+0]= A[id+0]+B[id+0];
			A[id+1]= A[id+1]+B[id+1];
			A[id+2]= A[id+2]+B[id+2];
			A[id+3]= A[id+3]+B[id+3];
		}
		double end = omp_get_wtime();
		printf(" %0.3f\t",(3 * 4 * 4 * 4) * ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0);
}


void MemoryTest_NoCacheLine(unsigned int i, float4* A, float4* B){
		unsigned int ITEMS  = pow(2,i);
		//Clear the cache!

		//Run benchmark
		double start = omp_get_wtime();
		#pragma omp parallel for
		for (unsigned int id = 0; id < ITEMS; id++) {
			A[id]= A[id]+B[id];
		}
		double end = omp_get_wtime();
		printf(" %0.3f\t",(3 * 4 * 4 * 4) * ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0);
}


int main(int argc, char *argv[]) {
	int thread_num = 1;
	bool single_test = false;
	double runs = 28;
	if (argc > 1) {	thread_num = atoi(argv[1]);}
	if (argc > 2 && string(argv[2])=="-s") {single_test = true; printf("Performing constant thread test\n");}
	
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

	float4* A = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );
	float4* B = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );
	float4* C = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );
	float4* D = (float4*) _mm_malloc (pow(2,runs)*sizeof(float4), 16 );


	int max_items = pow(2,runs);
	//Generate data
	#pragma omp parallel for 
	for (int i = 0; i < max_items; i++) {
		A[i] = float4(i);
		B[i] = float4(1.0/float(i));
		C[i] = float4(2.0/float(i));
		D[i] = float4(3.0/float(i));
	}


	for (int i = 14; i < runs; i++) {
		ClearCache(C,D,max_items);
		MemoryTest_CacheLine(i, A, B);
	}
	// for (int i = 14; i < runs; i++) {
	// 	ClearCache(C,D,max_items);
	// 	MemoryTest_NoCacheLine(i, A, B);
	// }

	printf("\n");
	_mm_free(A);
	_mm_free(B);
	_mm_free(C);
	_mm_free(D);
}

	return 0;
}
