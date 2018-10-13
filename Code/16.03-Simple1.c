int main(int argc, char **argv) {
  int me, size;

  /* Get options for MPI, register process, turn on deamon
   * (if needed), create sockets, etc.
   */
  MPI_Init(&argc, &argv);

  /* Define communicator (MPI_COMM_WORLD is global communicator,
   * organized in a complete graph
   */
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  /* Get number of process in the communicator. We can create new
   * ones as needed
   */
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  (void) printf("Process %d size %d\n", me, size);

  /* Processment... */

  /* Unregister process from daemon and finish data */
  MPI_Finalize();

  return 0;
}
