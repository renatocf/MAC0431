#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
  int message;
  int rank, size, tag, to, from;

  /* Start up MPI */
  MPI_Init(&argc, &argv); /* Set up MPI daemon, communication topology, etc */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* MPI_COMM_WORLD:                */
  MPI_Comm_size(MPI_COMM_WORLD, &size); /* communicator that includes all */
                                        /* processors and their channels  */

  /* Arbitrarily choose 201 to be our tag. Calculate the rank of the process in
   * the ring. Use the modulus operator so that the last process "wraps around"
   * to rank zero
   */

  message = 0;                     /* Initial message content */
  tag = 201;                       /* Tag for the message */
  to = (rank + 1) % size;          /* Destiny, to simulate a ring */
  from = (rank + size - 1) % size; /* Origin, to simulate a ring */

  /* If we are the "console" process, get an integer from the user to specify
   * how many times we want to go around the ring.
   */

  /* 0 is choosen as the initial, because it will work with any ring
   * with size >= 0.
   */
  if (rank == 0) {
    /* Send the message around the ring 5 times */
    message = 5;
    printf("Rank 0 starting message around the ring -- 1st of %d\n",
            message);
    MPI_Send(&message, 1, MPI_INT, to, tag, MPI_COMM_WORLD);
    /* MPI_INT takes care of converting big/little endian */
  }

  /* Pass the message around the ring. Teh exit mechanism wors as follows: the
   * message (a positive integer) is passed around the rind. Each time it pesses
   * rank 0, it is decremented. When each process receives the 0 message, it
   * passes it on.
   */

  while (1) {
    MPI_Recv(&message, 1, MPI_INT, from, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Rank %d received message \"%d\" from rank %d\n", rank, message, from);

    if (rank == 0) message--;

    MPI_Send(&message, 1, MPI_INT, to, tag, MPI_COMM_WORLD);

    if (message != 0) {
      printf("Rank %d sending message \"%d\" to rank %d\n", rank, message, to);
    }
    else {
      printf("Rank %d all done; quitting loop\n", rank);
      break;
    }
  }

  if (rank == 0) {
    MPI_Recv(&message, 1, MPI_INT, from, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    printf("Rank 0 consumed final message\n");
  }

  /* Quit */
  MPI_Finalize();

  return 0;
}
