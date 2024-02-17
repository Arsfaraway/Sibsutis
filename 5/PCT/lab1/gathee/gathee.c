#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {

int size, count=1024, rank, root=0;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);

double t = MPI_Wtime();

char* buf = malloc(sizeof(*buf)*count);


if (rank != root) {
        buf[0]='A';
        for (int i = 0; i < size; i++) {
                if (i == root) continue;
                MPI_Send(buf, count, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
}

else {
        MPI_Recv(buf, count, MPI_CHAR, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

t = MPI_Wtime()-t;
printf("time = %f\n", t); 
free(huf);
MPI_Finalize();
return 0;
}
