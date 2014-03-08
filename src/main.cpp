#include "omp.h"
#include <iostream>
#include <string>
#include <vector>
#include <math.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include "float4.h"
using namespace std;

unsigned int MAX_ITEMS  = pow(2,25);


inline void BandwidthFunction_float4( float4 * __restrict__ A, const float4 * __restrict__ B) {
#pragma omp parallel for
		for (int id = 0; id < MAX_ITEMS; id++) {
			A[id]= A[id]+B[id];
		}
}

int main(int argc, char *argv[]) {

	int thread_num = 1;
	if (argc > 1) {

		thread_num = atoi(argv[1]);
	}
	omp_set_num_threads(thread_num);
	// Length of vectors


	// Allocate memory for each vector on host
	float4* A = (float4 *)_mm_malloc(MAX_ITEMS*sizeof(float4), 16);
	float4* B = (float4 *)_mm_malloc(MAX_ITEMS*sizeof(float4), 16);
	// Initialize vectors on host
	#pragma omp parallel for 
	for (int i = 0; i < MAX_ITEMS; i++) {
		A[i] = float4(cosf(i) * cosf(i));
		B[i] = float4(cosf(i) * cosf(i));
	}

	double total_time_omp;
	double total_flops;
	double total_memory;
	double runs = 100;
	for (int i = 0; i < runs; i++) {
		double start = omp_get_wtime();
		
		#pragma omp parallel for
		for (int id = 0; id < MAX_ITEMS; id++) {
			A[id]= A[id]+B[id];
		}

		double end = omp_get_wtime();
		total_time_omp += (end - start) * 1000;
		total_flops += MAX_ITEMS / ((end - start)) / 1e9;

		total_memory += (3 * 4 * 4) * MAX_ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0; ;
		
	}
printf("\nExecution time in milliseconds =  %0.3f ms | %0.3f Gflop | %0.3f GB/s \n", total_time_omp/runs, total_flops/runs, total_memory/runs);
	_mm_free(A);
	_mm_free(B);
	return 0;
}
