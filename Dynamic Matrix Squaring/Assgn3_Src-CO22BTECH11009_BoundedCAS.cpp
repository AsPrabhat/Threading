#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

atomic<int> counter(0);
atomic<bool> lock(false);
const int MAX_RETRIES = 1000; // Maximum number of retries for acquiring the lock

void multiply(vector<vector<int>> &A, vector<vector<int>> &C, int N, int rowInc)
{
    bool expected = false;
    int retries = 0;
    int start_row, end_row;

    while (true)
    {
        while (!lock.compare_exchange_weak(expected, true, memory_order_acquire))
        {
            expected = false;
            retries++;
            if (retries >= MAX_RETRIES)
            {
                return;
            }
        }

        start_row = counter;
        end_row = min(start_row + rowInc, N);
        counter = end_row;

        lock.store(false, memory_order_release);

        if (start_row < N)
        {
            for (int i = start_row; i < end_row; i++)
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
        else
        {
            break;
        }
    }
}

void BoundedCAS(vector<vector<int>> &A, vector<vector<int>> &C, int N, int K, int rowInc)
{
    vector<thread> threads;

    for (int i = 0; i < K; i++)
    {
        threads.push_back(thread(multiply, ref(A), ref(C), N, rowInc));
    }

    for (vector<thread>::iterator th = threads.begin(); th != threads.end(); th++)
    {
        th->join();
    }
}

int main()
{
    int N, K, rowInc;
    ifstream input("inp.txt");

    input >> N;
    input >> K;
    input >> rowInc;

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
    BoundedCAS(A, C, N, K, rowInc);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    ofstream output("out_BoundedCAS.txt");
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