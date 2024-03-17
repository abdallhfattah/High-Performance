#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <stdbool.h>

int is_prime(int num) {
    if (num < 2) {
        return 0;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(int argc, char* argv[]) {
    int my_rank;        /* rank of process */
    int p;              /* number of process */
    int source;         /* rank of sender */
    int dest = 0;       /* rank of reciever */
    int tag = 0;        /* tag for messages */
    MPI_Status status;  /* return status for recieve */



    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    if (my_rank == 0) {
        int range;
        int low, high;

        printf("Enter first number: ");
        fflush(stdout);
        scanf("%d", &low);
        printf("Enter second number: ");
        fflush(stdout);
        scanf("%d", &high);

        range = (high - low + 1) / (p - 1 + (p == 1));
        for (int dest_rank = 1; dest_rank < p; dest_rank++) {
            MPI_Send(&low, 1, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
            MPI_Send(&range, 1, MPI_INT, dest_rank, tag, MPI_COMM_WORLD);
        }
    }
    else {
        int low, range;
        MPI_Recv(&low, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        MPI_Recv(&range, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

        int local_primes = 0;
        int lower_bound = low + (my_rank - 1) * range;
        int upper_bound = range + lower_bound;
        for (int i = lower_bound; i < upper_bound; i++) {
            if (is_prime(i)) {
                local_primes++;
            }
        }

        MPI_Send(&local_primes, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);

        printf("Total number of prime numbers in P%d is: %d\n", my_rank, local_primes);
        fflush(stdout);
    }

    if (my_rank == 0) {
        int count = 0;
        int total_count = 0;
        for (source = 1; source < p; source++) {
            MPI_Recv(&count, 1, MPI_INT, source, tag, MPI_COMM_WORLD,
                &status);

            total_count += count;
        }

        printf("Total number of prime numbers is: %d\n", total_count);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}
