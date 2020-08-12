#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std;
constexpr uint64_t magic_number = 12345;

void Add(int n, uint64_t *x)
{
    for (int i = 0; i < n; ++i)
        x[i] += x[i];
}

int main(void)
{
    int n = 1<<30;
    uint64_t *x = (uint64_t *)malloc(n * sizeof(uint64_t));
    memset(x, magic_number, sizeof(x)); 

    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();
    Add(n, x);
    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << " ms" << endl;

    free(x);
    return 0;
}