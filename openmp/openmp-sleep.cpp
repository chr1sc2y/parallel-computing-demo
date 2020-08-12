#include <omp.h>
#include <iostream>
#include <thread>
#include <chrono>

constexpr int thread_num = 3;
using namespace std; 

int main()
{
    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();

#pragma omp parallel for schedule(static) num_threads(thread_num)
    for (int i = 0; i < thread_num; ++i)    std::this_thread::sleep_for(std::chrono::seconds(1));

    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << " ms" << endl;

    return 0;
}