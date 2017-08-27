#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
  // Initialize the MPI environment
  MPI_Init(NULL, NULL);
  // Find out rank, size
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  int randomMessage;
  // Receive from the lower process and send to the higher process. Take care
  // of the special case when you there is a deadlock
  if (world_rank != 0) {
    MPI_Recv(&randomMessage, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received randomMessage %d from process %d\n", world_rank, randomMessage,
           world_rank - 1);
  } else {
    // Set the random message value if you are process 0
    randomMessage = -1;
  }
  MPI_Send(&randomMessage, 1, MPI_INT, (world_rank + 1) % world_size, 0,
           MPI_COMM_WORLD);
  // Now process 0 can receive from the last process. This makes sure that at
  // least one MPI_Send is initialized before all MPI_Recvs (again, to prevent
  // deadlock)
  if (world_rank == 0) {
    MPI_Recv(&randomMessage, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    printf("Process %d received randomMessage %d from process %d\n", world_rank, randomMessage,
           world_size - 1);
  }
  MPI_Finalize();
}