#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {

int count=1, rank, commsize, root;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &commsize);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

t = MPI_Wtime(void);

char* sbuf = malloc(sizeof(*buf)*count)
char* rbuf = malloc(sizeof(*buff)*count)

	sbuf[0] = 'a';
	rbuf[rank] = sbuf[0];
	
	for (int i = 0; i < commsize - 1; i++)
	{
		MPI_Send(rbuf, count, MPI_CHAR, (rank-i+commsize)%commsize, 0, MPI_COMM_WORLD);
		MPI_Recv(rbuf, count, MPI_CHAR, (rank-i-1-commsize)%commsize, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	}



/*
	buf[0]='A';
	MPI_Send(buf, count, MPI_CHAR, i+1, 0, MPI_COMM_WORLD);
	MPI_Recv(buff, count, MPI_CHAR, root, i-1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
*/	
	

/*
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
*/


t = MPI_Wtime()-t;
printf("time = %ld\n", t); 
free(buf);
MPI_Finalize();

