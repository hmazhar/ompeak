#include "omp.h"
#include <stdio.h>
#include <string>
#include <math.h>
#include <unistd.h>
#include <nmmintrin.h>
#include <assert.h>
using namespace std;
class __attribute__ ((aligned(16))) float4 {
public:
	__m128 mmvalue;
	inline float4() 		:mmvalue(_mm_setzero_ps()) { }
	inline float4(float a) 	:mmvalue(_mm_set1_ps(a)) {}
	inline float4(__m128 m) :mmvalue(m) {}
	operator __m128() const { return mmvalue;}
	inline float4 operator+(const float4& b) const {	return _mm_add_ps(mmvalue, b.mmvalue);}
	inline float4& operator+=(const float4& b) { *this = *this + b; return *this; }

};
	static inline float horizontal_add(const __m128 & a) {
	__m128 t1 = _mm_hadd_ps(a, a);
	__m128 t2 = _mm_hadd_ps(t1, t1);
	return _mm_cvtss_f32(t2);
}

void ClearCache(float4* C, float4* D, unsigned int max){
		float4 sum (0);
		#pragma omp parallel for 
		for (unsigned int i = 0; i < max; i+=4) {
			sum += C[i]+D[i];
		}
		//Prevents compiler from optimizing away sum
		assert(!_mm_testz_si128(sum, sum));

}

double MemoryTest_Write(unsigned int i, float4* A){
		unsigned int ITEMS  = pow(2,i);
		//Clear the cache!

		//Run benchmark
		double start = omp_get_wtime();
		#pragma omp parallel for
		for (unsigned int id = 0; id < ITEMS; id+=4) {
			A[id+0]= id;
			A[id+1]= id+1;
			A[id+2]= id+2;
			A[id+3]= id+3;
		}
		double end = omp_get_wtime();
		double bandwidth =(1 * 4 * 4 * 4) * ITEMS/4.0 / ((end - start)) / 1024.0 / 1024.0 / 1024.0;
		printf(" %0.3f\t",bandwidth);
		return bandwidth;
}

double MemoryTest_Read(unsigned int i, float4* A){
		unsigned int ITEMS  = pow(2,i);
		//Clear the cache!

		//Run benchmark
		double start = omp_get_wtime();

		float4 sum (0);

		#pragma omp parallel for
		for (unsigned int id = 0; id < ITEMS; id+=4) {
			sum += A[id+0]+A[id+1]+A[id+2]+A[id+3];
		}
		//Prevents compiler from optimizing away sum
		assert(!_mm_testz_si128(sum, sum));

		double end = omp_get_wtime();
		double bandwidth =(1 * 4 * 4 * 4) * ITEMS/4.0 / ((end - start)) / 1024.0 / 1024.0 / 1024.0;
		printf(" %0.3f\t",bandwidth);
		return bandwidth;
}


double MemoryTest_ReadWriteCacheLine(unsigned int i, float4* A, float4* B){
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
		double bandwidth = (3 * 4 * 4 * 4) * ITEMS/4.0 / ((end - start)) / 1024.0 / 1024.0 / 1024.0;
		printf(" %0.3f\t",bandwidth);
		return bandwidth;
}


double MemoryTest_ReadWriteNoCacheLine(unsigned int i, float4* A, float4* B){
		unsigned int ITEMS  = pow(2,i);
		//Clear the cache!

		//Run benchmark
		double start = omp_get_wtime();
		#pragma omp parallel for
		for (unsigned int id = 0; id < ITEMS; id++) {
			A[id]= A[id]+B[id];
		}
		double end = omp_get_wtime();
		double bandwidth =(3 * 4 * 4) * ITEMS / ((end - start)) / 1024.0 / 1024.0 / 1024.0;
		printf(" %0.3f\t",bandwidth);
		return bandwidth;
}


int main(int argc, char *argv[]) {
	int thread_num = 1;
	bool single_test = false;
	bool force_cache_line = true;
	int runs = 26;
	if (argc > 1) {	thread_num = atoi(argv[1]);}
	if (argc > 2){
		for(int args = 2; args < argc; args++){
			if(string(argv[args])=="-s"){
				single_test = true; printf("Performing constant thread test\n");
			}
			if(string(argv[args])=="-ncl") {
				force_cache_line = false; printf("Do not force cache line\n");
			}
		}
	}
	
	int max_items = pow(2,runs);

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

	float4* C = (float4*) malloc (max_items*sizeof(float4));
	float4* D = (float4*) malloc (max_items*sizeof(float4));

#pragma omp parallel for 
	for (int i = 0; i < max_items; i++) {
		C[i] = float4(2.0/float(i+1));
		D[i] = float4(3.0/float(i+1));
	}

double best_bandwidth , best_transfer_size, current_bandwidth;
int best_threads ;

best_bandwidth = best_transfer_size = best_threads = 0;
printf("Performing ReadWrite Test: \n");
for (int threads = start_threads; threads <= max_threads; threads++) {
		omp_set_num_threads(threads);
		printf("%3d\t", threads);

	float4* A = (float4*) malloc (max_items*sizeof(float4));
	float4* B = (float4*) malloc (max_items*sizeof(float4));

	//Generate data
	#pragma omp parallel for 
	for (int i = 0; i < max_items; i++) {
		A[i] = float4(i+1);
		B[i] = float4(1.0/float(i+1));
	}
	for (int i = 14; i < runs; i++) {
		ClearCache(C,D,max_items);
		if(force_cache_line){
			current_bandwidth = MemoryTest_ReadWriteCacheLine(i, A, B);
		}else{
			current_bandwidth = MemoryTest_ReadWriteNoCacheLine(i, A, B);
		}
		if(best_bandwidth<current_bandwidth){
			best_bandwidth = current_bandwidth;
			best_threads = threads;
			best_transfer_size = sizeof(float4)*pow(2,i)/1024.0/1024.0;
		}

	}
	printf("\n");
	free(A);
	free(B);

}
printf("Best: %d threads, \t%0.3f GB/s, \t%0.3f MB \n",best_threads, best_bandwidth,best_transfer_size);


best_bandwidth = best_transfer_size = best_threads = 0;
printf("Performing Read Test: \n");

for (int threads = start_threads; threads <= max_threads; threads++) {
		omp_set_num_threads(threads);
		printf("%3d\t", threads);

	float4* A = (float4*) malloc (max_items*sizeof(float4));
	float* B = (float*) malloc (max_items*sizeof(float));
	//Generate data
	#pragma omp parallel for 
	for (int i = 0; i < max_items; i++) {
		A[i] = float4(i+1);
	}
	for (int i = 14; i < runs; i++) {
		ClearCache(C,D,max_items);
		current_bandwidth = MemoryTest_Read(i, A);

		if(best_bandwidth<current_bandwidth){
			best_bandwidth = current_bandwidth;
			best_threads = threads;
			best_transfer_size = sizeof(float4)*pow(2,i)/1024.0/1024.0;
		}
	}
	printf("\n");
	free(A);
	free(B);
}

printf("Best: %d threads, \t%0.3f GB/s, \t%0.3f MB \n",best_threads, best_bandwidth,best_transfer_size);


best_bandwidth = best_transfer_size = best_threads = 0;
printf("Performing Write Test: \n");

for (int threads = start_threads; threads <= max_threads; threads++) {
	omp_set_num_threads(threads);
	printf("%3d\t", threads);

	float4* A = (float4*) malloc (max_items*sizeof(float4));

	//Generate data
	#pragma omp parallel for 
	for (int i = 0; i < max_items; i++) {
		A[i] = float4(i+1);
	}
	for (int i = 14; i < runs; i++) {

		ClearCache(C,D,max_items);
		current_bandwidth = MemoryTest_Write(i, A);

		if(best_bandwidth<current_bandwidth){
			best_bandwidth = current_bandwidth;
			best_threads = threads;
			best_transfer_size = sizeof(float4)*pow(2,i)/1024.0/1024.0;
		}
	}
	printf("\n");
	free(A);

}
printf("Best: %d threads, \t%0.3f GB/s, \t%0.3f MB \n",best_threads, best_bandwidth,best_transfer_size);
	free(C);
	free(D);
	return 0;
}
