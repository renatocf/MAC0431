#include <stdio.h>
#include <stdlib.h>

long int TAM_VET, VEZES;
/* Um vetor para o host, outro para a placa */
double *vet, *vet_d;

__global__ void calcula(double *vet_d, long int TAM_VET) {
    long int idx = blockIdx.x * blockDim.x + threadIdx.x;

    /* Para não passar dos limites */
    if (idx >= TAM_VET) return;
    vet_d[idx] += cos(idx * sqrt(idx * 1.0) * 4.0);
}

int main(int argc, char **argv) {
  int B = 1000, T = 1024; /* Blocos e threads */

  if ((vet = malloc(TAM_VET * sizeof(double))) != NULL) {
    fprintf(stderr, "Could not alloc block of size %d in the host",
            TAM_VET * sizeof(double));
  }

  if (cudaMalloc(&vet_d, TAM_VET * sizeof(double)) != cudaSuccess) {
    fprintf(stderr, "Could not alloc block of size %d in the device",
            TAM_VET * sizeof(double));
  }

  if (cudaMemcpy(vet_d, vet, TAM_VET*sizeof(double),
      cudaMemcpyHostToDevice) != cudaSuccess) {
    fprintf(stderr, "Could not copy block of size %d in the device",
            TAM_VET * sizeof(double));
  }

  cudaFree(vet_d);
  free(vet);
}
