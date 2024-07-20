#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <chrono>
#include <atomic>

using namespace std;

atomic<int> counter(0);
atomic_flag lock = ATOMIC_FLAG_INIT;

void multiply(vector<vector<int>> &A, vector<vector<int>> &C, int N, int rowInc)
{
    int start_row, end_row;

    while (true)
    {
        while (lock.test_and_set(memory_order_acquire))
        {
            // Spinning
        }

        start_row = counter;
        end_row = min(start_row + rowInc, N);
        counter = end_row;

        lock.clear(memory_order_release);

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

void TAS(vector<vector<int>> &A, vector<vector<int>> &C, int N, int K, int rowInc)
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
    TAS(A, C, N, K, rowInc);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    ofstream output("out_TAS.txt");
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