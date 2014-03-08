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
#define DIV  16.0
#define FETCH_2(sum, id, A)  sum += A[id];   id ++;   sum += A[id];   id ++;
#define FETCH_8(sum, id, A)  FETCH_2(sum, id, A); FETCH_2(sum, id, A);   FETCH_2(sum, id, A);   FETCH_2(sum, id, A);

void BandwidthFunction_float4(float4 * __restrict__ A, float * __restrict__ B) {
//#pragma omp parallel for schedule(static)

		//float4 *A = (float4*) __builtin_assume_aligned(a, 16);
		//float *B = (float*)__builtin_assume_aligned(b, 16);


		for (int id = 0; id < MAX_ITEMS-DIV; id+=DIV) {

			float4 sum = 0;
			sum += A[id+0];
			sum += A[id+1];
			sum += A[id+2];
			sum += A[id+3];
			sum += A[id+4];
			sum += A[id+5];
			sum += A[id+6];
			sum += A[id+7];
			sum += A[id+8];
			sum += A[id+9];
			sum += A[id+10];
			sum += A[id+11];
			sum += A[id+12];
			sum += A[id+13];
			sum += A[id+14];
			sum += A[id+15];


	__m128 t1 = _mm_hadd_ps(sum.mmvalue, sum.mmvalue);
	__m128 t2 = _mm_hadd_ps(t1, t1);


			B[id]= _mm_cvtss_f32(t2);
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
