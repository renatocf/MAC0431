int main(int argc, char **argv) {
  int me, size;
  int SOME_TAG = 0;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &me);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if ((me % 2) == 0) { /* send unless highest-numbered process */
    if ((me + 1) < size)
      mpi_send(/* message */, me + 1, SOME_TAG, MPI_COMM_WORLD);
  }
  else {
    mpi_recv(/* message */, me - 1, SOME_TAG, MPI_COMM_WORLD);
  }

  MPI_Finalize();

  return 0;
}
