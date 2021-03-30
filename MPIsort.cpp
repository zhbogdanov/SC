#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>
#include <clocale>

#define N 300000
double startT, stopT;

void ParallelBubbleSort(int *arr, int size)
{
    for (int i = 0; i < size; ++i)
    {
        int tmp = 0;
        if (i % 2 == 0)
        {
            for (int j = 0; j < size; j += 2)
                if (j < size - 1)
                    if(arr[j] > arr[j + 1])
                    {
                        tmp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = tmp;
                    }
        }
        else
        {
            for (int j = 1; j < size; j += 2)
                if (j < size - 1)
                    if(arr[j] > arr[j + 1])
                    {
                        tmp = arr[j];
                        arr[j] = arr[j + 1];
                        arr[j + 1] = tmp;
                    }
        }
    }
}

int main(int argc, char **argv)
{
        int *data;
        int *chunk;
        int *other;
        int m, id, p, s, i, step, n=N;
        MPI_Status status;

        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &id);
        MPI_Comm_size(MPI_COMM_WORLD, &p);

        if(id == 0)
        {
                int r = n % p;
                s = n / p;
                data = (int *)malloc((n + p - r) * sizeof(int));
                for(i = 0; i < n; ++i)
                        data[i] = random();
                if(r != 0)
                {
                        for(i = n; i < n + p - r; ++i)
                           data[i] = 0;
                        s = s + 1;
                }
                startT = MPI_Wtime();
                MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
                chunk = (int *)malloc(s * sizeof(int));
                MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
                ParallelBubbleSort(chunk, s);
        }
        else
        {
                MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
				chunk = (int *)malloc(s * sizeof(int));
                MPI_Scatter(data, s, MPI_INT, chunk, s, MPI_INT, 0, MPI_COMM_WORLD);
                ParallelBubbleSort(chunk, s);
        }

    	if(id == 0)
        {
                stopT = MPI_Wtime();
                printf("%d; %d processors; %f secs\n", N, p, stopT - startT);
        }
        MPI_Finalize();
}
