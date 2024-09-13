#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void saveMatrixBinary(const char* filename, int* matrix, int mSize) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fwrite(&mSize, sizeof(int), 1, file);  // Save matrix size
    fwrite(matrix, sizeof(int), mSize * mSize, file);  // Save matrix data
    fclose(file);
}

void saveMatrixText(const char* filename, int* matrix, int mSize) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    fprintf(file, "%d\n", mSize);  // Save matrix size
    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++) {
            fprintf(file, "%d ", matrix[i * mSize + j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void generateRandomAdjMatrix(int* matrix, int mSize, float probability) {
    int i, j;
    int maxWeight = mSize * mSize;  // Max weight for the edges

    // Initialize the adjacency matrix with 0 (no edges initially)
    for (i = 0; i < mSize; i++) {
        for (j = 0; j < mSize; j++) {
            matrix[i * mSize + j] = 0;
        }
    }

    // Fill the upper triangular part of the matrix with random edges based on probability
    for (i = 0; i < mSize; i++) {
        for (j = i + 1; j < mSize; j++) {
            // Determine if an edge should be created based on probability
            if ((float)rand() / RAND_MAX < probability) {
                int weight = rand() % maxWeight + 1;  // Random weights between 1 and maxWeight
                matrix[i * mSize + j] = weight;
                matrix[j * mSize + i] = weight;  // Symmetric matrix (undirected graph)
            }
        }
    }
}

void printMatrix(int* matrix, int mSize) {
    for (int i = 0; i < mSize; i++) {
        for (int j = 0; j < mSize; j++) {
            printf("%4d ", matrix[i * mSize + j]);
        }
        printf("\n");
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <number_of_vertices> <probability> <output_filename>\n", argv[0]);
        return 1;
    }

    int mSize = atoi(argv[1]);
    float probability = atof(argv[2]);
    const char* filename = argv[3];

    if (mSize <= 0 || probability < 0 || probability > 1) {
        fprintf(stderr, "Error: Invalid number of vertices or probability\n");
        return 1;
    }

    srand(time(NULL));  // Seed the random number generator

    // Allocate memory for the adjacency matrix
    int* matrix = (int*)malloc(mSize * mSize * sizeof(int));
    if (matrix == NULL) {
        perror("Error allocating memory");
        return 1;
    }

    // Generate the random adjacency matrix with edges based on the probability
    generateRandomAdjMatrix(matrix, mSize, probability);

    // Print the matrix generated (optional)
    printf("Adjacency matrix with edges based on probability %.2f:\n", probability);
    // printMatrix(matrix, mSize);

    // Save the matrix to the specified binary file
    saveMatrixBinary(filename, matrix, mSize);

    // Free allocated memory
    free(matrix);

    return 0;
}