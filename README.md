OMPeak
======

An OpenMP benchmarking utility

The purpose of this benchmark is to test peak performance of OpenMP code using very simple tests. 


##Usage:
This program has one argument which takes the number of threads, if it is not specified the program will run single threaded. 

~~~

./ompeak #threads

~~~



##Output:

The program outputs information in the following way:

~~~

Execution time =  0.423 ms | Memory Transfered:  0.250| Bandwidth  1.732 GB/s 
Execution time =  0.520 ms | Memory Transfered:  0.500| Bandwidth  2.817 GB/s 
Execution time =  0.532 ms | Memory Transfered:  1.000| Bandwidth  5.508 GB/s 
Execution time =  0.999 ms | Memory Transfered:  2.000| Bandwidth  5.865 GB/s 
Execution time =  1.944 ms | Memory Transfered:  4.000| Bandwidth  6.028 GB/s 
Execution time =  3.583 ms | Memory Transfered:  8.000| Bandwidth  6.541 GB/s 
Execution time =  6.610 ms | Memory Transfered:  16.000| Bandwidth  7.092 GB/s 
Execution time =  15.069 ms | Memory Transfered:  32.000| Bandwidth  6.221 GB/s 
Execution time =  51.096 ms | Memory Transfered:  64.000| Bandwidth  3.670 GB/s 
Execution time =  119.154 ms | Memory Transfered:  128.000| Bandwidth  3.147 GB/s 
Execution time =  227.084 ms | Memory Transfered:  256.000| Bandwidth  3.303 GB/s

~~~
