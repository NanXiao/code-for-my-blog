#include <stdlib.h>
#include <stdio.h>

// This will output the proper CUDA error strings in the event that a CUDA host call returns an error
#define checkCudaErrors(err)           __checkCudaErrors (err, __FILE__, __LINE__)

inline void __checkCudaErrors(cudaError err, const char *file, const int line)
{
    if (cudaSuccess != err)
    {
        fprintf(stderr, "%s(%i) : CUDA Runtime API error %d: %s.\n",
                file, line, (int)err, cudaGetErrorString(err));
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char **argv)
{
    printf("[%s] - Starting...\n", argv[0]);
    int gpuid[2] = {1, 2}; // we want to find the first two GPU's that can support P2P
    float total_time = 0.0;
    bool enable_p2p = true;

    if (enable_p2p)
    {
        // Enable peer access
        printf("Enabling peer access between GPU%d and GPU%d...\n", gpuid[0], gpuid[1]);
        checkCudaErrors(cudaSetDevice(gpuid[0]));
        checkCudaErrors(cudaDeviceEnablePeerAccess(gpuid[1], 0));
        checkCudaErrors(cudaSetDevice(gpuid[1]));
        checkCudaErrors(cudaDeviceEnablePeerAccess(gpuid[0], 0));
    }

    for (int loop = 0; loop < 100; loop++)
    {
        // Allocate buffers
        const size_t buf_size = 1024 * 1024 * 16 * sizeof(float);
        printf("Allocating buffers (%iMB on GPU%d, GPU%d and CPU Host)...\n", int(buf_size / 1024 / 1024), gpuid[0], gpuid[1]);
        checkCudaErrors(cudaSetDevice(gpuid[0]));
        float *g0;
        checkCudaErrors(cudaMalloc(&g0, buf_size));
        checkCudaErrors(cudaSetDevice(gpuid[1]));
        float *g1;
        checkCudaErrors(cudaMalloc(&g1, buf_size));

        // Create CUDA event handles
        cudaEvent_t start_event, stop_event;
        float time_memcpy;
        int eventflags = cudaEventBlockingSync;
        checkCudaErrors(cudaEventCreateWithFlags(&start_event, eventflags));
        checkCudaErrors(cudaEventCreateWithFlags(&stop_event, eventflags));

        // P2P memcopy() benchmark
        checkCudaErrors(cudaEventRecord(start_event, 0));

        for (int i = 0; i < 100; i++)
        {
            // With UVA we don't need to specify source and target devices, the
            // runtime figures this out by itself from the pointers

            // Ping-pong copy between GPUs
            if (i % 2 == 0)
            {
                checkCudaErrors(cudaMemcpy(g1, g0, buf_size, cudaMemcpyDefault));
            }
            else
            {
                checkCudaErrors(cudaMemcpy(g0, g1, buf_size, cudaMemcpyDefault));
            }
        }

        checkCudaErrors(cudaEventRecord(stop_event, 0));
        checkCudaErrors(cudaEventSynchronize(stop_event));
        checkCudaErrors(cudaEventElapsedTime(&time_memcpy, start_event, stop_event));
        total_time += time_memcpy;
        printf("cudaMemcpyPeer / cudaMemcpy (%f ms) between GPU%d and GPU%d: %.2fGB/s\n", time_memcpy, gpuid[0], gpuid[1],
               (1.0f / (time_memcpy / 1000.0f)) * ((100.0f * buf_size)) / 1024.0f / 1024.0f / 1024.0f);

        // Free resources
        checkCudaErrors(cudaEventDestroy(start_event));
        checkCudaErrors(cudaEventDestroy(stop_event));
        checkCudaErrors(cudaSetDevice(gpuid[0]));
        checkCudaErrors(cudaFree(g0));
        checkCudaErrors(cudaSetDevice(gpuid[1]));
        checkCudaErrors(cudaFree(g1));
    }


    if (enable_p2p)
    {
        // Disable peer access (also unregisters memory for non-UVA cases)
        printf("Disabling peer access...\n");
        checkCudaErrors(cudaSetDevice(gpuid[0]));
        checkCudaErrors(cudaDeviceDisablePeerAccess(gpuid[1]));
        checkCudaErrors(cudaSetDevice(gpuid[1]));
        checkCudaErrors(cudaDeviceDisablePeerAccess(gpuid[0]));
    }

    printf("Total time is %.2fs\n", total_time / 1000);
    //delete device_handler;
    return (EXIT_SUCCESS);
}
