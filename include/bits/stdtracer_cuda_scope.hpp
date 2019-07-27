#pragma once
#include <chrono>
#include <string>

#include <cuda_runtime.h>

template <typename ctx_t, typename clock_t = std::chrono::high_resolution_clock>
class basic_cuda_scope
{
    const std::string name_;
    ctx_t &ctx_;

    cudaEvent_t start, stop;

  public:
    basic_cuda_scope(const std::string &name, ctx_t &ctx)
        : name_(name), ctx_(ctx)
    {
        ctx_.in(name_);
        cudaEventCreate(&start);
        cudaEventCreate(&stop);
        cudaEventRecord(start);
    }

    ~basic_cuda_scope()
    {
        cudaEventRecord(stop);
        cudaEventSynchronize(stop);
        float ms;
        // https://docs.nvidia.com/cuda/cuda-runtime-api/group__CUDART__EVENT.html
        cudaEventElapsedTime(&ms, start, stop);
        cudaEventDestroy(start);
        cudaEventDestroy(stop);

        using namespace std::chrono_literals;
        std::chrono::milliseconds unit = 1ms;
        ctx_.out(name_, ms * unit);
    }
};
