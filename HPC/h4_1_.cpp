#include <iostream>
#include <cstdlib>
#include <cuda_runtime.h>

// Kernel function for vector addition
__global__ void vectorAddition(const int* a, const int* b, int* result, int size) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;

    if (tid < size) {
        result[tid] = a[tid] + b[tid];
    }
}

int main() {
    int size = 10000;
    int* host_a = new int[size];
    int* host_b = new int[size];
    int* host_result = new int[size];

    // Initialize input vectors
    for (int i = 0; i < size; ++i) {
        host_a[i] = i;
        host_b[i] = i;
    }

    // Allocate device memory
    int* device_a;
    int* device_b;
    int* device_result;

    cudaMalloc((void**)&device_a, size * sizeof(int));
    cudaMalloc((void**)&device_b, size * sizeof(int));
    cudaMalloc((void**)&device_result, size * sizeof(int));

    // Copy input vectors from host to device
    cudaMemcpy(device_a, host_a, size * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(device_b, host_b, size * sizeof(int), cudaMemcpyHostToDevice);

    // Define block and grid dimensions
    int blockSize = 256;
    int gridSize = (size + blockSize - 1) / blockSize;

    // Launch kernel for vector addition
    vectorAddition<<<gridSize, blockSize>>>(device_a, device_b, device_result, size);

    // Copy result vector from device to host
    cudaMemcpy(host_result, device_result, size * sizeof(int), cudaMemcpyDeviceToHost);

    // Verify the result
    for (int i = 0; i < size; ++i) {
        if (host_result[i] != host_a[i] + host_b[i]) {
            std::cout << "Error: Vector addition result is incorrect!\n";
            break;
        }
    }

    std::cout << "Vector addition completed successfully!\n";

    // Free device memory
    cudaFree(device_a);
    cudaFree(device_b);
    cudaFree(device_result);

    // Free host memory
    delete[] host_a;
    delete[] host_b;
    delete[] host_result;

    return 0;
}
