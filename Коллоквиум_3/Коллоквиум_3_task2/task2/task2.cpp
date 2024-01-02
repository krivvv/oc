#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <fstream>
using namespace std;
mutex matrixMutex;
vector<vector<int>> matrixA, matrixB, matrixC;
int m, n, k, p, threadsCompleted = 0;

void initializeMatrix(vector<vector<int>>& matrix, int rows, int cols) {
    matrix.resize(rows, vector<int>(cols));
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            cout << "Enter element [" << i << "][" << j << "]: ";
            cin >> matrix[i][j];
        }
    }
}

void calculateProduct(int threadId) {
    for (int j = 0; j < k; ++j) {
        for (int i = 0; i < m; ++i) {
            {
                lock_guard<mutex> lock(matrixMutex);
                if (matrixC[i][j] == -1) {
                    matrixC[i][j] = 0;
                    for (int x = 0; x < n; ++x) {
                        matrixC[i][j] += matrixA[i][x] * matrixB[x][j];
                    }
                    cout << "Thread " << threadId << ": C[" << i << "][" << j << "] = " << matrixC[i][j] << endl;
                }
            }
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
    {
        lock_guard<mutex> lock(matrixMutex);
        threadsCompleted++;
    }
}

int main() {
    ofstream out("protocol.txt");
    cout << "Enter the number of rows for matrix A: ";
    cin >> m;
    cout << "Enter the number of columns for matrix A: ";
    cin >> n;
    cout << "Enter the number of columns for matrix B: ";
    cin >> k;
    cout << "Enter the number of threads: ";
    cin >> p;
    cout << "Enter elements for matrix A:" << endl;
    initializeMatrix(matrixA, m, n);
    cout << "Enter elements for matrix B:" << endl;
    initializeMatrix(matrixB, n, k);
    matrixC = vector<vector<int>>(m, vector<int>(k, -1));
    vector<thread> threads;
    for (int i = 0; i < p; ++i) {
        threads.emplace_back(calculateProduct, i);
    }

    while (threadsCompleted < p) {
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    for (auto& thread : threads) {
        thread.join();
    }
    cout << "Matrix C:" << endl;
    for (const auto& row : matrixC) {
        for (int element : row) {
            cout << element << " ";
        }
        cout << endl;
    }
    for (const auto& row : matrixC) {
        for (int element : row) {
            out << element << " ";
        }
        out << endl;
    }
    out.close();
    return 0;
}
