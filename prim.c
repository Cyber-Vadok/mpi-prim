#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <stdbool.h>

FILE *f_result;
FILE *f_time;

typedef struct { int v1; int v2; } Edge;

void loadMatrixBinary(const char *filename, int **matrix, int *mSize);

int main(int argc, char *argv[]){
    
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int mSize;
    int *matrix = NULL;

    if(rank == 0){
        loadMatrixBinary("matrix.bin", &matrix, &mSize);
        if (mSize == -1){
            fprintf(stderr, "Error loading matrix\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&mSize, 1, MPI_INT, 0, MPI_COMM_WORLD);

    int * displs = (int *)malloc(sizeof(int) * size);    // Displacements for each process
    int * sendcounts = (int *)malloc(sizeof(int) * size); // Number of elements to send to each process

    int rows_per_proc = mSize / size;    // Base number of rows per process
    int extra_rows = mSize % size;       // Number of extra rows to distribute

    // For each process, calculate the send counts and displacements
    for (int i = 0; i < size; i++) {
        // If this process gets an extra row, it receives (rows_per_proc + 1) rows
        if (i < extra_rows) {
            sendcounts[i] = (rows_per_proc + 1) * mSize;
        } else {
            sendcounts[i] = rows_per_proc * mSize;
        }

        // Displacement: sum of the sendcounts of previous processes
        if (i == 0) {
            displs[i] = 0; // First process starts at index 0
        } else {
            displs[i] = displs[i - 1] + sendcounts[i - 1];
        }
    }

    // Determine the number of rows this process will receive
    int local_rows = (rank < extra_rows) ? (rows_per_proc + 1) : rows_per_proc;
    int * local_chunk = (int *)malloc(sizeof(int) * local_rows*mSize);

    // Now you can use MPI_Scatterv to distribute the matrix
    MPI_Scatterv(matrix, sendcounts, displs, MPI_INT, local_chunk, local_rows * mSize, MPI_INT, 0, MPI_COMM_WORLD);

    int * parent = (int *)malloc(mSize * sizeof(int));
    int * key = (int *)malloc(mSize * sizeof(int));
    bool * mstSet = (bool *)malloc(mSize * sizeof(bool));

    int totalWeight = 0;

    for (int i = 0; i < mSize; i++){
        key[i] = INT_MAX;
        mstSet[i] = false;
        parent[i] = -1;
    }  

    key[0] = 0;
    int va = 0;
    int offset = displs[rank] / mSize;

    // Synchronize all processes before starting the timer
    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int k = 0 ; k < mSize - 1; k++){
        
        mstSet[va] = true;
        
        for (int j = 0; j < local_rows; j++){

            int global_row = j + offset;
            int edge_weight = local_chunk[j*mSize + va];

            if(edge_weight && mstSet[global_row] == false && edge_weight < key[global_row]){
                parent[global_row] = va;
                key[global_row] = edge_weight;
            }
        }

        int min = INT_MAX;
        int vb = 0;

        for (int i = 0; i < local_rows; i++){
            int global_row = i + offset;
            if (mstSet[global_row] == false && key[global_row] < min){
                min = key[global_row];
                vb = global_row;
            }
        }

        struct { int weight; int rank; } minWR, localWR;
        Edge edge;

        localWR.weight = min;
        localWR.rank = rank;

        MPI_Allreduce(&localWR, &minWR, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
        
        edge.v1 = parent[vb];
        edge.v2 = vb;

        MPI_Bcast(&edge, 1, MPI_2INT, minWR.rank, MPI_COMM_WORLD);

        if (minWR.weight != INT_MAX){
            totalWeight += minWR.weight;
            key[edge.v2] = minWR.weight;
            parent[edge.v2] = edge.v1;
            va = edge.v2; //new
        }
    }

    // Synchronize all processes before stopping the timer
    MPI_Barrier(MPI_COMM_WORLD);
    double finish, calc_time; 
    finish = MPI_Wtime();
    calc_time = finish-start;

    if (rank == 0) {
        f_result = fopen("Result.txt", "w");
        fprintf(f_result,"The min minWeight is %d\n", totalWeight);
        for (int i=1; i< mSize; ++i) {
            fprintf(f_result,"Edge %d %d\n",i, parent[i]);
        }
        fclose(f_result);

        f_time = fopen("Time.txt", "a+");
        fprintf(f_time, "\n Number of processors: %d\n Number of vertices: %d\n Time of execution: %f\n Total Weight: %d\n\n", size, mSize ,calc_time, totalWeight);
        fclose(f_time);
        printf("\n Number of processors: %d\n Number of vertices: %d\n Time of execution: %f\n Total Weight: %d\n\n", size, mSize ,calc_time, totalWeight);
    }

    if (rank == 0){
        free(matrix);
    }

    // Clean up allocated memory
    free(parent);
    free(key);
    free(mstSet);
    free(local_chunk);
    free(sendcounts);
    free(displs);

    MPI_Finalize();
    return 0;
}

// The function to load the matrix from a binary file
void loadMatrixBinary(const char *filename, int **matrix, int *mSize)
{
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
    {
        perror("Error opening file");
        *mSize = -1;  // Indicate error
        return;
    }

    // Read matrix size
    if (fread(mSize, sizeof(int), 1, file) != 1)
    {
        perror("Error reading matrix size");
        fclose(file);
        *mSize = -1;  // Indicate error
        return;
    }

    // Allocate memory for matrix
    *matrix = (int *)malloc(*mSize * *mSize * sizeof(int));
    if (*matrix == NULL)
    {
        perror("Error allocating memory");
        fclose(file);
        *mSize = -1;  // Indicate error
        return;
    }

    // Read matrix data
    if (fread(*matrix, sizeof(int), *mSize * *mSize, file) != *mSize * *mSize)
    {
        perror("Error reading matrix data");
        free(*matrix);
        fclose(file);
        *mSize = -1;  // Indicate error
        return;
    }

    fclose(file);
}