OMPeak
======

An OpenMP benchmarking utility

The purpose of this benchmark is to test peak performance of OpenMP code using very simple tests. 


##Usage:
This program has one argument which takes the number of threads, if it is not specified the program will run single threaded. 

~~~

./ompeak #threads (-s) (-ncl)

-s: run a single test with the number of threads specified
-ncl: do not force the code to use an entire cache line

~~~

##Output:

The program outputs information in the following way:
A table is generated with each column showing the size of memory transferred (MBytes) and the rows showing the number of openMP threads.

Three tests are performed, for each test the performance, optimal thread number and memory transfer size are shown

~~~

./ompeak 4
Memory transfered in MBytes (column) for a certain number of threads (rows): 
   	 0.25	 0.50	 1.00	 2.00	 4.00	 8.00	 16.00	 32.00	 64.00	 128.00	 256.00	 512.00	
Performing ReadWrite Test: 
  1	 5.545	 5.408	 11.495	 12.083	 11.433	 13.036	 12.097	 12.156	 11.250	 11.597	 9.618	 3.289	
  2	 7.474	 12.539	 13.371	 14.913	 17.210	 16.707	 16.345	 17.806	 18.247	 17.868	 18.160	 6.164	
  3	 7.719	 11.357	 14.011	 13.885	 14.850	 15.159	 16.043	 16.302	 16.669	 16.769	 17.038	 5.640	
  4	 8.214	 11.793	 14.222	 12.681	 14.931	 16.343	 17.484	 17.161	 17.403	 17.945	 17.771	 5.734	
Best: 2 threads, 	18.247 GB/s, 	64.000 MB 
Performing Read Test: 
  1	 5.206	 7.302	 8.434	 9.430	 8.850	 10.051	 10.261	 10.514	 10.712	 10.811	 10.762	 10.900	
  2	 3.056	 7.503	 7.582	 9.693	 8.418	 11.911	 12.326	 13.659	 14.272	 13.922	 14.547	 14.798	
  3	 2.981	 4.761	 5.674	 9.057	 11.039	 10.836	 13.893	 15.169	 16.114	 16.081	 16.643	 16.447	
  4	 2.848	 5.243	 7.516	 8.297	 10.295	 12.651	 13.301	 16.324	 16.863	 16.425	 10.884	 12.876	
Best: 2 threads, 	18.247 GB/s, 	64.000 MB 
Performing Write Test: 
  1	 8.982	 9.394	 8.411	 9.256	 9.718	 9.181	 8.964	 8.726	 8.687	 8.413	 8.467	 8.619	
  2	 2.301	 5.082	 7.817	 9.526	 10.304	 10.067	 9.442	 9.284	 9.397	 9.029	 9.262	 9.022	
  3	 4.000	 4.481	 7.642	 9.858	 10.145	 10.459	 8.758	 8.635	 7.180	 8.015	 7.324	 8.657	
  4	 2.653	 5.432	 7.628	 8.875	 10.416	 10.266	 6.973	 8.911	 9.108	 6.073	 8.770	 8.964	
Best: 2 threads, 	18.247 GB/s, 	64.000 MB 

~~~
