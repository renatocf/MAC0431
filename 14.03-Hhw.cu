#include "util/cuPrintf.cu"
#include <stdio.h>
#include <stdlib.h>

/**
 * For all functions, we can have 3 different access levels:
 * __host__   : function to be executed by the host, default.
 * __global__ : function to be executed by the device (GPU),
 *              but can be called by the host.
 * __device__ : function to be executed only by the device,
 *              not accessible from the host.
 */

__global__ void device_greetings(void)
{
    cuPrintf("Hello, world from the device! (%d, %d) (%d, %d)",
            blockIdx.x, blockIdx.y, threadIdx.x, threadIdx.y);
}

int main(int argc, char **argv) {
  int Gx = 1, Gy = 1, Bx = 1, By = 1;

  printf("Hello, world from the host");

  // Initialize printf inside device
  cudaPrintfInit();

  // Grade dimensions
  dim3 Grade(Gx, Gy);
  dim3 Blocks(Bx, By);

  // Execute function for Grade and Block
  device_greetings<<<Grade, Blocks>>>();

  // Display in the host output from device
  cudaPrintfDisplay();

  // End output from device
  cudaPrintfEnd();

  return 0;
}
