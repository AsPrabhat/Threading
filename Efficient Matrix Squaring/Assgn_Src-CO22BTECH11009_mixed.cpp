#include<iostream>
#include<vector>
#include<fstream>
#include<thread>
#include<chrono>

using namespace std;

void multiply(vector<vector<int>> &A, vector<vector<int>> &C, int N, int threadID, int K)
{
    for (int i = threadID; i < A.size(); i += K)
    {
        for (int j = 0; j < A.size(); j++)
        {
            C[i][j] = 0;
            for (int k = 0; k < A.size(); k++)
            {
                C[i][j] += A[i][k] * A[k][j];
            }
        }
    }
}

void mixed(int N, int K, vector<vector<int>> &A, vector<vector<int>> &C)
{
    vector<thread> threads;

    for (int i = 0; i < K; i++)
    {
        threads.push_back(thread(multiply, ref(A), ref(C), N, i, K));
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
    mixed(N, K, A, C);

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    ofstream output("out_mixed.txt");
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