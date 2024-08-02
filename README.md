# Threading

### 1. Project 1 - Efcient matrix squaring with different threading techniques
This project aims to perform parallel matrix multiplication in C++. Consider a square matrix A. The goal of the problem is to find the square of matrix A in parallel.

Here two techniques are applied for computing the resultant matrix in parallel:

1. Chunk: The resultant matrix is divided into chunks of size p = N/K. Then thread1 will compute the values of the rows 1 to p of C; thread2 will compute the values of the rows p + 1 to 2 * p; thread3 will compute the values of the rows 2*p + 1 to 3*p and so on. Thus, the thread thi will be responsible for computing the rows corresponding to chunk i.

2. Mixed: Here, the rows of the resultant matrix are evenly distributed among the threads. Thread1 will be responsible for the following rows of the C matrix: 1, k+1, 2*k+1 ....,. Similarly, thread2 will be responsible for the following rows of the C matrix: 2, k+2, 2*k+2 ....,. This pattern continues for all the threads.

### 2. Project 2 - Using thread affinity in matrix multiplication
To evaluate the efficiency of a parallel matrix squaring algorithm by studying the impact of assigning individual threads to CPU cores. In this project, I've built upon my previous work measuring the performance of calculating the square of matrix A in parallel in C++.

### 3. Project 3 - Dynamic matrix squaring
This assignment aims to perform parallel matrix multiplication through a Dynamic mechanism in C++. This assignment is an extension of the project 1. Consider a square matrix A. The goal of the problem is to find the square of matrix A in parallel using the dynamic mechanism.

Also, different mutual exclusion algorithms are implemented, which are: (a) TAS, (b) CAS, (c) Bounded CAS, and (d) atomic increment (provided by the C++ atomic library).
