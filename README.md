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
  1	 8.416	 10.539	 5.838	 12.006	 12.587	 10.921	 12.874	 12.218	 8.814	 8.822	 10.706	 4.236	
  2	 4.279	 4.804	 12.840	 14.832	 15.624	 16.343	 8.361	 8.947	 11.327	 16.779	 14.974	 4.916	
  3	 8.236	 13.684	 14.805	 16.787	 15.584	 18.084	 17.716	 11.655	 13.499	 16.926	 17.505	 5.832	
  4	 5.428	 10.685	 7.529	 15.663	 16.600	 16.696	 16.783	 16.549	 17.645	 17.518	 16.486	 5.964	
Best: 3 threads, 	18.084 GB/s, 	8.000 MB 
Performing Read Test: 
  1	 5.523	 7.302	 8.044	 8.015	 8.508	 9.695	 10.173	 10.655	 10.910	 10.652	 9.995	 10.706	
  2	 3.163	 5.637	 6.919	 8.434	 9.628	 10.249	 11.792	 13.366	 13.541	 14.488	 14.791	 14.731	
  3	 3.014	 4.846	 8.500	 9.143	 11.188	 11.644	 12.972	 15.443	 15.643	 16.292	 16.836	 16.671	
  4	 2.981	 4.250	 5.929	 7.575	 10.976	 12.064	 12.739	 15.443	 15.007	 16.938	 15.817	 15.565	
Best: 4 threads, 	16.938 GB/s, 	128.000 MB 
Performing Write Test: 
  1	 9.062	 9.752	 9.660	 9.918	 9.621	 9.389	 8.681	 8.625	 8.579	 8.444	 8.305	 8.395	
  2	 3.807	 3.821	 7.237	 8.308	 10.389	 9.939	 10.022	 9.301	 9.215	 9.088	 9.097	 9.182	
  3	 3.170	 6.187	 8.275	 10.436	 10.015	 10.214	 9.680	 9.226	 9.097	 9.256	 8.964	 8.756	
  4	 2.775	 4.697	 4.068	 9.438	 9.966	 9.990	 9.063	 9.076	 8.893	 8.771	 8.809	 8.076	
Best: 3 threads, 	10.436 GB/s, 	2.000 MB

~~~
