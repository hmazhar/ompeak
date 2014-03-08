#include "omp.h"
#include <iostream>
#include <vector>
#include <math.h>
//#include <nmmintrin.h>
using namespace std;
#include "float4.h"

// class __attribute__ ((aligned(16))) float4 {
// public:
// 	union {
// 		struct {float x, y, z, w;};
// 		__m128 mmvalue;
// 	};
// 	inline float4() 		:mmvalue(_mm_setzero_ps()) { }
// 	inline float4(float a) 	:mmvalue(_mm_set1_ps(a)) {}
// 	inline float4(__m128 m) :mmvalue(m) {}
// 	inline float4 operator+(const float4& b) const {	return _mm_add_ps(mmvalue, b.mmvalue);}
// };

int main(int argc, char *argv[]) {
	int thread_num = 1;
	if (argc > 1) {	thread_num = atoi(argv[1]);}
	omp_set_num_threads(thread_num);

	double runs = 27;
	vector<float4> A (pow(2,runs));
	vector<float4> B (pow(2,runs));
	vector<float4> C (pow(2,runs));
	vector<float4> D (pow(2,runs));
	//Generate data
	// #pragma omp parallel for 
// 	for (int i = 0; i < 134217728; i++) {
// 		A[i] = float4(1.0*i);
// 		B[i] = float4(1.3/float(i));
// 		//C[i] = float4(1.3/float(i));
// 		//D[i] = float4(1.5/float(i));
// 	}


	for (int i = 14; i < runs; i++) {
		unsigned int MAX_ITEMS  = pow(2,i);
		//Clear the cache!
		#pragma omp parallel for 
		for (unsigned int i = 0; i < 134217728; i+=4) {
			C[i] = D[i]+C[i];
		}
		//Run code
		double start = omp_get_wtime();
		#pragma omp parallel for
		for (int id = 0; id < MAX_ITEMS; id++) {
			A[id+0]= A[id+0]+B[id+0];
			A[id+1]= A[id+1]+B[id+1];
			A[id+2]= A[id+2]+B[id+2];
			A[id+3]= A[id+3]+B[id+3];
		}
		double end = omp_get_wtime();
		printf("Execution time =  %0.3f ms | Memory Transfered:  %0.3f| Bandwidth  %0.3f GB/s \n", (end - start) * 1000, sizeof(float4)*MAX_ITEMS/1024.0/1024.0, (3 * 4 * 4) * MAX_ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0);
	}
	A.clear();
	B.clear();
	C.clear();
	D.clear();
	return 0;
}
