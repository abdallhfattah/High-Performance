#include <mpi.h>
#include <stdio.h>

void CaesarCipher(char word[], int currentRank, int numberOfProcesses, int currentProcessSize,int flag) {
    int start = (currentRank - 1) * (currentProcessSize);
    int end = (currentRank) * (currentProcessSize);
    if (currentRank == numberOfProcesses - 1 ) {
        end = strlen(word);
    }
    if (flag) {
        for (int i = start; i < end; i++) {
            if (word[i] >= 'x') {
                word[i] -= 26;
            }
            word[i] += 3;
        }
    } else {
        for (int i = start; i < end; i++) {
            if (word[i] <= 'c') {
                word[i] += 26;
            }
            word[i] -= 3;
        }
    }
}

int main(int argc, char * argv[])
{
    // Initialize the MPI environment
    MPI_Init(&argc, &argv);
    int currentRank;
    int numberOfProcesses;
    int flag;
    char word[100] = "tutorial";
    MPI_Comm_rank(MPI_COMM_WORLD, &currentRank);
    MPI_Comm_size(MPI_COMM_WORLD, &numberOfProcesses);
    int currentProcessSize = strlen(word) / (numberOfProcesses - 1 + (numberOfProcesses == 1));
    if (currentRank == 0) {
        int choice;
        printf("What do you want to do?\n1. Encrypt\n2. Decrypt\n");
        fflush(stdout);
        scanf("%d", &choice);
        if (choice == 1) {
            flag = 1;
            printf("Please enter the word you want to encrypt: ");
            fflush(stdout);
        } else {
            flag = 0;
            printf("Please enter the word you want to decrypt: ");
            fflush(stdout);
        }
        scanf("%s", word);

        for (int i = 1; i < numberOfProcesses; i++) {
            MPI_Send(&word, strlen(word), MPI_CHAR, i, 0, MPI_COMM_WORLD);
            MPI_Send(&flag, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&word, 100, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        CaesarCipher(word, currentRank, numberOfProcesses, currentProcessSize, flag);
        MPI_Send(&word, strlen(word), MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    if (currentRank == 0) {
        char encryptedWord[100];
        for(int i = 1; i < numberOfProcesses; i++) {
            MPI_Recv(&word, 100, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int start = (i - 1) * (currentProcessSize);
            int end = i * (currentProcessSize);
            if (i == numberOfProcesses - 1) {
                end = strlen(word);
            }   
            for (int i = start; i < end; i++) {
                encryptedWord[i] = word[i];
            }
        }
        printf("%s\n", encryptedWord);
        fflush(stdout);
    }


    MPI_Finalize();
}

