#include <cuda_runtime.h>
#include <iostream>
#include <limits>

__constant__ __device__ int prog[16] = {2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 6, 5, 5, 3, 0};

__global__ void check_num_kernel(unsigned long long n, unsigned long long* result) {
    unsigned long long r0 = n + blockIdx.x * blockDim.x + threadIdx.x;
    unsigned long long r1 = 0;
    unsigned long long r2 = 0;
    int step              = 0;

    while (step < 16) {
        r1 = r0 % 8;
        r1 ^= 5;
        r2 = r0 >> r1;
        r1 ^= 6;
        r0 >>= 3;
        r1 ^= r2;
        if (r1 % 8 != prog[step]) {
            break;
        }
        if (r0 == 0)
            break;

        ++step;
    }

    if (step == 15) {
        *result = n + blockIdx.x * blockDim.x + threadIdx.x;
    }
}

int main() {
    // char* h_results = (char*)malloc(1024 * 32);
    unsigned long long* h_result = new unsigned long long(0);
    unsigned long long* d_result;
    cudaMalloc(&d_result, sizeof(unsigned long long));
    // 35184372088832
    for (size_t i = 106904920099226; i < std::numeric_limits<size_t>::max(); i += 1024ul * 2147483647) {
        // for (size_t i = 0; i < 1024 * 32 * 10; i += 1024 * 32) {
        cudaMemset(d_result, 0, sizeof(unsigned long long));
        check_num_kernel<<<2147483647, 1024>>>(i, d_result);
        cudaDeviceSynchronize();
        cudaMemcpy(h_result, d_result, sizeof(unsigned long long), cudaMemcpyDeviceToHost);
        if (*h_result != 0) {
            std::cout << *h_result << std::endl;
            break;
        }
    }
    delete h_result;
    cudaFree(d_result);
    cudaError_t err = cudaGetLastError();
    if (err != cudaSuccess)
        std::cout << "CUDA error: " << cudaGetErrorString(err) << std::endl;
    // cudaProfilerStop();
}
