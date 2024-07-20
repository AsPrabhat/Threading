#include<iostream>
#include<vector>
#include<fstream>
#include<thread>
#include<chrono>
#include<sched.h>

using namespace std;

void multiply(vector<vector<int>> &A, vector<vector<int>> &B, int N, int threadID, int K)
{
    for (int i = threadID; i < A.size(); i += K)
    {
        for (int j = 0; j < A.size(); j++)
        {
            B[i][j] = 0;
            for (int k = 0; k < A.size(); k++)
            {
                B[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

void mixed(int N, int K, int C, vector<vector<int>> &A, vector<vector<int>> &B, int BT)
{
    vector<thread> threads;
    cpu_set_t cpuset;

    for (int i = 0; i < K; i++)
    {
        CPU_ZERO(&cpuset);
        int core_id = i % C; // Assign thread to different core
        CPU_SET(core_id, &cpuset);
        
        threads.push_back(thread([=, &A, &B, &BT, &N]() {
            if (BT > 0)
            {
                pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset); // Set thread affinity
                BT--;
            }
            multiply(A, B, N, i, K);
        }));
    }

    for(vector<thread>::iterator th = threads.begin(); th != threads.end(); th++)
    {
        th -> join();
    }
}

int main()
{
    int N, K, C, BT;
    ifstream input("inp.txt");

    input >> N;
    input >> K;
    input >> C;
    input >> BT;

    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> B(N, vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            input >> A[i][j];
        }
    }

    auto start = chrono::high_resolution_clock::now();
    mixed(N, K, C, A, B, BT);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    ofstream output("out_mixed.txt");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            output << B[i][j] << " ";
        }
        output << endl;
    }

    output << "Time taken: " << duration.count() << " microseconds" << endl;

    input.close();
    output.close();
    return 0;
}