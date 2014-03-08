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

int main(int argc, char *argv[]) {

	int thread_num = 1;
	if (argc > 1) {

		thread_num = atoi(argv[1]);
	}
	omp_set_num_threads(thread_num);
	// Length of vectors

	double runs = 27;
	// Allocate memory for each vector on host
	float4* A = (float4 *)_mm_malloc(pow(2,runs)*sizeof(float4), 16);
	float4* B = (float4 *)_mm_malloc(pow(2,runs)*sizeof(float4), 16);
	// Initialize vectors on host

	double total_time_omp;
	double total_flops;
	double total_memory;
	
	for (int i = 19; i < runs; i++) {
unsigned int MAX_ITEMS  = pow(2,i);
	#pragma omp parallel for 
	for (int i = 0; i < MAX_ITEMS; i++) {
		A[i] = float4(1.0);
		B[i] = float4(1.3);
	}
		unsigned int split_size = pow(2,19);
		unsigned int splits = MAX_ITEMS/split_size;
	
		double start = omp_get_wtime();
		for(int i=0; i<splits; i++){
			#pragma omp parallel for
			for (int id = i*split_size; id < (i+1)*(split_size); id++) {
				A[id]= A[id]+B[id];
			}
		}
		double end = omp_get_wtime();
		total_time_omp = (end - start) * 1000;
		total_flops = MAX_ITEMS / ((end - start)) / 1e9;

		total_memory = (3 * 4 * 4) * MAX_ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0; ;
		printf("Execution time in milliseconds =  %0.3f ms | %0.3f|  %0.3f GB/s \n", total_time_omp, sizeof(float4)*MAX_ITEMS/1024.0/1024.0, total_memory);

	}
	_mm_free(A);
	_mm_free(B);
	return 0;
}
