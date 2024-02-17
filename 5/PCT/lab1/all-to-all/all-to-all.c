#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {

int count=1, rank, commsize, root;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &commsize);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

t = MPI_Wtime(void);

char* buf = malloc(sizeof(*buf)*count)
char* buff = malloc(sizeof(*buff)*count)

MPI_Request requests[1];
MPI_Status statuses[1];

	buf[0]='A';
	for (int i = 0; i < size; i++) {
		if (rank != i) 
			MPI_Isend(buf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, &requests[0]);
			MPI_Waitall(count, requests, statuses);
	}

	for (int i = 0; i < size; i++) {
		if (rank != i) 
			MPI_Irecv(buff, count, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE, &requests[0]);
			MPI_Waitall(count, &requests[0], &statuses[0]);
	}



t = MPI_Wtime()-t;
printf("time = %ld\n", t); 
free(buf);
MPI_Finalize();

