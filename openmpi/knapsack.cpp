#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>
#include <omp.h>

using namespace std;

int main()
{
    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();
    
    fstream input_file("input-knapsack.txt");
    int N;
    int64_t Capacity;
    input_file >> N >> Capacity;
    int64_t weight[N], value[N];
    for (int i = 0; i < N; ++i)
        input_file >> weight[i] >> value[i];
    

    vector<vector<int64_t>> dp(N + 1, vector<int64_t>(Capacity + 1));
    for (int i = 0; i <= N; ++i)
    {
        #pragma omp parallel for
        for (int64_t j = 0; j <= Capacity; ++j)
        {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (j < weight[i - 1])
                dp[i][j] = dp[i - 1][j];
            else
                dp[i][j] = max(dp[i - 1][j], dp[i - 1][j - weight[i - 1]] + value[i - 1]);
        }
    }
    cout << "max value: " << dp[N][Capacity] << endl;

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << endl;
    return EXIT_SUCCESS;
}