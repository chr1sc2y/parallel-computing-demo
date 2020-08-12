#include <iostream>
#include <cstdlib>
#include <cstring>
#include <chrono>
#include <thread>
#include <string>

using namespace std;
constexpr int magic_number = 12345;

__global__ void Add(int n, int *cuda_x)
{
    int index = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x;
    for (int i = index; i < n; i += stride)
        cuda_x[i] += cuda_x[i];
}

int main(void)
{
    int n = 1<<30;
    int64_t byte_size = n * sizeof(int);
    int *x;
    x = (int*)malloc(byte_size);
    for (int i = 0; i < n; ++i)
        x[i] = magic_number;
    
    int *cuda_x;
    cudaMalloc((void**)&cuda_x, byte_size);

    // copy from host to device
    cudaMemcpy(cuda_x, x, byte_size, cudaMemcpyHostToDevice);

    std::chrono::steady_clock::time_point time_begin = std::chrono::steady_clock::now();
    Add<<<1, 1024>>>(n, cuda_x);
    cudaDeviceSynchronize();
    std::chrono::steady_clock::time_point time_end = std::chrono::steady_clock::now();
    
    // copy from device to host
    cudaMemcpy(x, cuda_x, byte_size, cudaMemcpyDeviceToHost);

    // check result
    bool result{ true };
    for (uint32_t i = 0; i < n; ++i)
        result = (result && (x[i] == magic_number + magic_number));
    string result_str = (result ? "true" : "false");

    cout << "result: " << result_str << endl;
    cout << "time: " << std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_begin).count() << " ms" << endl;
    free(x);
    cudaFree(cuda_x);
    return 0;
}