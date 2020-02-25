#include <iostream>
#include <numeric>
#include <vector>

#include <cuda_runtime.h>

#include <tracer/cuda>

DEFINE_TRACE_CONTEXTS;

template <typename R>
__global__ void kern(R *x, int n)
{
    for (int i = 0; i < n; ++i) { x[i] = i; }
}

template <typename R = int>
void f(int n)
{
    TRACE_SCOPE(__func__);
    void *data;
    cudaMalloc(&data, n * sizeof(R));
    R *x = (R *)data;
    kern<R><<<1, 1>>>(x, n);
    std::vector<R> y(n);
    cudaMemcpy(y.data(), data, n * sizeof(R), cudaMemcpyDeviceToHost);
    cudaFree(data);

    const R sum = std::accumulate(y.begin(), y.end(), 0);
    std::cout << sum << std::endl;
}

int main()
{
    TRACE_SCOPE(__func__);
    for (int i = 0; i < 10; ++i) { f((i + 1) * 1024); }
    return 0;
}
