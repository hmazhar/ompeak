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
	if (argc > 1) {	thread_num = atoi(argv[1]);}
	omp_set_num_threads(thread_num);

	double runs = 27;
	vector<float4> A (pow(2,runs));
	vector<float4> B (pow(2,runs));
	vector<float4> C (pow(2,runs));
	vector<float4> D (pow(2,runs));

	double total_time_omp;

	double total_memory;
	
	for (int i = 14; i < runs; i++) {
		unsigned int MAX_ITEMS  = pow(2,i);
		//Generate data
		#pragma omp parallel for 
		for (int i = 0; i < MAX_ITEMS; i++) {
			A[i] = float4(1.0*i);
			B[i] = float4(1.3/float(i));
		}
		//Clear the cache!
		#pragma omp parallel for 
		for (unsigned int i = 0; i < 134217728; i++) {
			C[i] = D[i]+C[i];
		}
		//Run code
		double start = omp_get_wtime();
		#pragma omp parallel for
		for (int id = 0; id < MAX_ITEMS; id+=4) {
			A[id+0]= A[id+0]+B[id+0];
			A[id+1]= A[id+1]+B[id+1];
			A[id+2]= A[id+2]+B[id+2];
			A[id+3]= A[id+3]+B[id+3];
		}
		double end = omp_get_wtime();
		total_time_omp = (end - start) * 1000;
		total_memory = (3 * 4 * 4) * MAX_ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0; ;
		printf("Execution time [ms] =  %0.3f ms | %0.3f|  %0.3f GB/s \n", total_time_omp, sizeof(float4)*MAX_ITEMS/1024.0/1024.0, total_memory);
	}
	A.clear();
	B.clear();
	C.clear();
	D.clear();
	return 0;
}
