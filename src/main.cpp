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

#define MAX_ITEMS  1024000*4
#define DIV  16
#define FETCH_2(sum, id, A)  sum += A[id];   id ++;   sum += A[id];   id ++;
#define FETCH_8(sum, id, A)  FETCH_2(sum, id, A); FETCH_2(sum, id, A);   FETCH_2(sum, id, A);   FETCH_2(sum, id, A);

void BandwidthFunction_float4(float4 * A, float * B) {
#pragma omp parallel for
		for (int id = 0; id < MAX_ITEMS-DIV; id+=8) {
			float4 sum = 0;
			sum += A[id+0];
			sum += A[id+1];
			sum += A[id+2];
			sum += A[id+3];
			sum += A[id+4];
			sum += A[id+5];
			sum += A[id+6];
			sum += A[id+7];
			B[id]= sum.x+sum.y+sum.z+sum.w;
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
	vector<float4> A_f4(MAX_ITEMS);
	vector<float>     B(MAX_ITEMS);
	// Initialize vectors on host
	for (int i = 0; i < MAX_ITEMS; i++) {
		A_f4[i] = float4(cosf(i) * cosf(i));
	}

	double total_time_omp;
	double total_flops;
	double total_memory;
	double runs = 100;
	for (int i = 0; i < runs; i++) {
		double start = omp_get_wtime();
		BandwidthFunction_float4(A_f4.data(), B.data());
		double end = omp_get_wtime();
		total_time_omp += (end - start) * 1000;
		total_flops += 4 * MAX_ITEMS / ((end - start)) / 1e9;

		total_memory += (DIV * 4 * 4) * MAX_ITEMS/DIV/ ((end - start)) / 1024.0 / 1024.0 / 1024.0; ;
		
	}
printf("\nExecution time in milliseconds =  %0.3f ms | %0.3f Gflop | %0.3f GB/s \n", total_time_omp/runs, total_flops/runs, total_memory/runs);
	A_f4.clear();
	B.clear();

	return 0;
}
