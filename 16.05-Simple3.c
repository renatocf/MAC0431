int main(int argc, char **argv) {
  int me, size, count;
  void *data;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  /* Processment... */

  if (me == 0) { /* get input for buffer "data" */ }

  /* Broadcast data to all processors */
  MPI_Bcast(data, count, MPI_BYTE, 0, MPI_COMM_WORLD);

  MPI_Finalize();

  return 0;
}
