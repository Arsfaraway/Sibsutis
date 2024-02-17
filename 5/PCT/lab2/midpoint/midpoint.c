#include <math.h>
#include <mpi.h>
#include <stdio.h>

double func(double x) { return sin(x + 2) / (0.4 + cos(x)); }

int main(int argc, char **argv) {
    double start_time, end_time;

    int commsize, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &commsize);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double eps = 1E-6;
    int n0 = 1000000;
    int n = n0, k;
    double sq[2] = {0.0, 0.0}, delta = 1; //Массив для хранения значений интеграла
    double a = -1.0, b = 1.0;

    start_time = MPI_Wtime();

    for (k = 0; delta > eps; n *= 2, k ^= 1) {  //Цикл для итеративного увеличения количества точек интегрирования. 
                                                //Переключение между буферами sq происходит через k ^= 1.
        int points_per_proc = n / commsize;
        int lb = rank * points_per_proc;
        int ub = (rank == commsize - 1) ? (n - 1) : (lb + points_per_proc - 1);
        double h = (b - a) / n; //Шаг интегрирования.
        double s = 0.0; //Локальная переменная для хранения суммы значений функции на текущем процессе.

        for (int i = lb; i <= ub; i++) {
            s += func(a + h * (i + 0.5));
            //Цикл для вычисления частичной суммы значений функции на текущем процессе.
        }
 операцию сбора, но результат отправляется на все процессы в коммуникаторе.
        MPI_Allreduce(&s, &sq[k], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD); //Обмен и суммирование частичных сумм всех процессов.
        //для выполнения операции сбора (reduction) над данными, распределенными между процессами. В данном случае, операция сбора - это суммирование.

        /*
        &s - адрес начала локальных данных, которые нужно собрать.
        &sq[k] - адрес начала буфера, в который будут собраны результаты операции сбора. Это буфер, который хранит частичную сумму значений функции 
            для текущего процесса.
        1 - количество элементов в данных для каждого процесса (в данном случае, суммируется только одно значение).
        MPI_DOUBLE - тип данных, который передается в операции сбора (в данном случае, тип double).
        MPI_SUM - операция сбора, которая будет выполнена (в данном случае, суммирование).
        MPI_COMM_WORLD - коммуникатор, определяющий группу процессов, участвующих в операции.

        После выполнения MPI_Allreduce, все процессы будут иметь в буфере &sq[k] глобальную сумму значений функции, вычисленную по всем процессам. 
        Это позволяет эффективно собирать результаты вычислений, распределенных по процессам MPI.
        */
        sq[k] *= h;
        if (n > n0) {
            delta = fabs(sq[k] - sq[k ^ 1]) / 3.0;
        }
    }

    end_time = MPI_Wtime();

    double elapsed_time = end_time - start_time;
    double max_elapsed_time;
операцию сбора и отправляет результат только на процесс с указанным рангом 
    MPI_Reduce(&elapsed_time, &max_elapsed_time, 1, MPI_DOUBLE, MPI_MAX, 0,
               MPI_COMM_WORLD); //Сбор и нахождение максимального времени выполнения.

    if (rank == 0) {
        printf("Result: %.12f; Runge rule: EPS %e, n %d\n", sq[k], eps, n / 2);
        printf("Max elapsed time: %f seconds\n", max_elapsed_time);
    }

    MPI_Finalize(); // Завершение MPI.
    return 0;
}
