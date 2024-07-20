#include<iostream>
#include<vector>
#include<fstream>
#include<thread>
#include<chrono>

using namespace std;

void multiply(vector<vector<int>> &A, vector<vector<int>> &C, int N, int start, int end)
{
    for (int i = start; i < end; i++)
    {
        for (int j = 0; j < N; j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < N; k++)
            {
                C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

void chunk(int N, int K, vector<vector<int>> &A, vector<vector<int>> &C)
{
    int p = N/K;
    vector<thread> threads;

    for (int i = 0; i < K; i++)
    {
        int start = i * p;
        int end = (i == K - 1) ? N : start + p;
        threads.push_back(thread(multiply, ref(A), ref(C), N, start, end));
    }

    for(vector<thread>::iterator th = threads.begin(); th != threads.end(); th++)
    {
        th -> join();
    }
}

int main()
{
    int N, K;
    ifstream input("inp.txt");

    input >> N;
    input >> K;

    vector<vector<int>> A(N, vector<int>(N));
    vector<vector<int>> C(N, vector<int>(N));
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            input >> A[i][j];
        }
    }

    auto start = chrono::high_resolution_clock::now();
    chunk(N, K, A, C);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    ofstream output("out_chunk.txt");
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            output << C[i][j] << " ";
        }
        output << endl;
    }

    output << "Time taken: " << duration.count() << " microseconds" << endl;
    
    input.close();
    output.close();
    return 0;
}