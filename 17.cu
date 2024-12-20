#include <chrono>
#include <cuda_runtime.h>
#include <iostream>

/// this configuration (2000000ul*32*1024) need about 1 seconds in my GTX 1060 6GB
/// find the result need about 34minutes,😄
const uint64_t chucksize = 2000000ul;
const uint gridsize      = 32;
const uint blocksize     = 1024;

__constant__ __device__ uint64_t prog[16] = {2, 4, 1, 5, 7, 5, 1, 6, 0, 3, 4, 6, 5, 5, 3, 0};

__global__ void check_num_kernel(uint64_t start, uint64_t* d_result, bool* d_found, uint64_t* d_found_i) {
    uint64_t tid     = blockIdx.x * blockDim.x + threadIdx.x;
    uint64_t start_i = start + tid * chucksize;

    if (*d_found && start_i > *d_found_i)
        return;

    for (uint64_t i = start_i; i < start_i + chucksize; ++i) {
        uint64_t r0 = i;
        uint64_t r1 = 0;
        uint64_t r2 = 0;
        int step    = 0;

        while (step < 16) {
            r1 = r0 % 8;
            r1 ^= 5;
            r2 = r0 >> r1;
            r1 ^= 6;
            r0 >>= 3;
            r1 ^= r2;
            if ((r1 & 7) != prog[step]) {
                break;
            }
            if (r0 == 0)
                break;

            ++step;
        }

        if (step == 15) {
            atomicMin((unsigned long long*)d_result, i);
            *d_found   = true;
            *d_found_i = i;
        }
    }
}

int main() {
    bool* h_found = new bool(false);
    bool* d_found;

    uint64_t* h_found_i = new uint64_t(UINT64_MAX);
    uint64_t* d_found_i;

    uint64_t* h_result = new uint64_t(UINT64_MAX);
    uint64_t* d_result;

    cudaMalloc(&d_result, sizeof(uint64_t));
    cudaMalloc(&d_found, sizeof(bool));
    cudaMalloc(&d_found_i, sizeof(uint64_t));
    cudaMemcpy(d_result, h_result, sizeof(uint64_t), cudaMemcpyHostToDevice);
    cudaMemcpy(d_found, h_found, sizeof(bool), cudaMemcpyHostToDevice);
    cudaMemcpy(d_found_i, h_found_i, sizeof(uint64_t), cudaMemcpyHostToDevice);

    std::cout << "batch size: " << gridsize * blocksize * chucksize << std::endl;
    auto starttime = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < UINT64_MAX; i += gridsize * blocksize * chucksize) {
        if (*h_found && i > *h_found_i) {
            break;
        }
#ifndef NDEBUG
        auto cuda_start_time = std::chrono::high_resolution_clock::now();
#endif
        check_num_kernel<<<gridsize, blocksize>>>(i, d_result, d_found, d_found_i);
        cudaDeviceSynchronize();

        cudaError_t err = cudaGetLastError();
        if (err != cudaSuccess)
            std::cout << "CUDA error: " << cudaGetErrorString(err) << std::endl;

        cudaMemcpy(h_result, d_result, sizeof(uint64_t), cudaMemcpyDeviceToHost);
        cudaMemcpy(h_found, d_found, sizeof(bool), cudaMemcpyDeviceToHost);
        cudaMemcpy(h_found_i, d_found_i, sizeof(uint64_t), cudaMemcpyDeviceToHost);

#ifndef NDEBUG
        auto cuda_end_time = std::chrono::high_resolution_clock::now();
        auto durationms    = std::chrono::duration_cast<std::chrono::milliseconds>(cuda_end_time - cuda_start_time);
        std::cout << durationms.count() << "ms" << " " << i << std::endl;
#endif
    }

    std::cout << *h_result << std::endl;
    auto endtime    = std::chrono::high_resolution_clock::now();
    auto durationms = std::chrono::duration_cast<std::chrono::minutes>(endtime - starttime);
    std::cout << durationms.count() << " minutes" << std::endl;

    delete h_result;
    delete h_found;
    delete h_found_i;
    cudaFree(d_result);
    cudaFree(h_found);
    cudaFree(h_found_i);
    return 0;
}
