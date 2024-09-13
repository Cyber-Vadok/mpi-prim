#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/types.h>
#include <time.h>

#define INF INT_MAX

void loadMatrixBinary(const char *filename, int **matrix, int *mSize);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <matrix_file>\n", argv[0]);
        return 1;
    }

    const char *filename = argv[1];
    int *matrix = NULL;
    int mSize;
    loadMatrixBinary(filename, &matrix, &mSize);

    if (mSize <= 0 || matrix == NULL) {
        fprintf(stderr, "Error loading matrix\n");
        return 1;
    }

    // Initialize variables for Prim's algorithm
    int *parent = (int *)malloc(mSize * sizeof(int)); // Stores the MST
    int *key = (int *)malloc(mSize * sizeof(int));    // Weight of the edge
    int *inMST = (int *)malloc(mSize * sizeof(int));  // True if vertex is included in MST
    int minWeight = 0;

    clock_t start_time = clock();  // Start clock here

    for (int i = 0; i < mSize; i++) {
        key[i] = INF;      // Initialize all keys as INFINITE
        inMST[i] = 0;      // Initialize all vertices as not in MST
        parent[i] = -1;    // Initialize parent of each vertex
    }

    // Start with the first vertex
    key[0] = 0;
    parent[0] = -1; // The first node is the root of the MST

    for (int count = 0; count < mSize - 1; count++) {
        // Find the minimum key vertex from the set of vertices not yet included in MST
        int min = INF, min_index = 0; // = 0 to remove the warning not necessary
        for (int v = 0; v < mSize; v++) {
            if (inMST[v] == 0 && key[v] < min) {
                min = key[v];
                min_index = v;
            }
        }

        // Add the selected vertex to the MST
        inMST[min_index] = 1;

        // Update the key value of adjacent vertices of the selected vertex
        for (int v = 0; v < mSize; v++) {
            if (matrix[min_index * mSize + v] && inMST[v] == 0 && matrix[min_index * mSize + v] < key[v]) {
                key[v] = matrix[min_index * mSize + v];
                parent[v] = min_index;
            }
        }
    }

    clock_t end_time = clock();  // End clock here

    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time taken by Prim's algorithm (excluding I/O): %f seconds\n", time_taken);

    // Print the MST
    // printf("Edge   Weight\n");
    for (int i = 1; i < mSize; i++) {
        // printf("%d - %d    %d\n", parent[i], i, matrix[i * mSize + parent[i]]);
        minWeight += matrix[i * mSize + parent[i]];
    }

    printf("Total weight of MST: %d\n", minWeight);

    // Free allocated memory
    free(matrix);
    free(parent);
    free(key);
    free(inMST);

    return 0;
}

void loadMatrixBinary(const char *filename, int **matrix, int *mSize) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error opening file");
        *mSize = -1; // Indicate error
        return;
    }

    // Read matrix size
    if (fread(mSize, sizeof(int), 1, file) != 1) {
        perror("Error reading matrix size");
        fclose(file);
        *mSize = -1; // Indicate error
        return;
    }

    // Allocate memory for matrix
    *matrix = (int *)malloc(*mSize * *mSize * sizeof(int));
    if (*matrix == NULL) {
        perror("Error allocating memory");
        fclose(file);
        *mSize = -1; // Indicate error
        return;
    }

    // Read matrix data
    if (fread(*matrix, sizeof(int), *mSize * *mSize, file) != *mSize * *mSize) {
        perror("Error reading matrix data");
        free(*matrix);
        fclose(file);
        *mSize = -1; // Indicate error
        return;
    }

    fclose(file);
}
