#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {

int count, rank, commsize, root;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &commsize);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

t = MPI_Wtime(void);

char* huf = malloc(sizeof(*buf)*count)


if (rank == root) {
	buf[0]='A';
	for (int i = 0; i < size; i++) {
		if (i == root) countinue;
		MPI_Send(buf, count, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
	}
}

else {
	MPI_Recv(buf, count, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

t = MPI_Wtime()-t;
printf("time = %ld\n", t); 
free(buf);
MPI_Finalize();

