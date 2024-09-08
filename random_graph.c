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
    int maxWeight = mSize * mSize;  // Valore massimo per il peso

    // Inizializza la matrice di adiacenza con 0 (nessun arco all'inizio)
    for (i = 0; i < mSize; i++) {
        for (j = 0; j < mSize; j++) {
            matrix[i * mSize + j] = 0;
        }
    }

    // Riempie la parte superiore della matrice con archi casuali in base alla probabilità
    for (i = 0; i < mSize; i++) {
        for (j = i + 1; j < mSize; j++) {
            // Determina se creare un arco con una certa probabilità
            if ((float)rand() / RAND_MAX < probability) {
                int weight = rand() % maxWeight + 1;  // Pesi casuali distinti tra 1 e maxWeight
                matrix[i * mSize + j] = weight;
                matrix[j * mSize + i] = weight;  // Matrice simmetrica (grafo non orientato)
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

int main() {
    int mSize;
    float probability;
    srand(time(NULL));

    printf("Inserisci la dimensione della matrice di adiacenza (numero di vertici): ");
    scanf("%d", &mSize);
    
    printf("Inserisci la probabilità di esistenza di un arco (tra 0 e 1): ");
    scanf("%f", &probability);

    // Allocazione della matrice di adiacenza
    int* matrix = (int*)malloc(mSize * mSize * sizeof(int));

    // Genera la matrice di adiacenza casuale con archi distinti in base alla probabilità
    generateRandomAdjMatrix(matrix, mSize, probability);

    // Stampa la matrice generata
    printf("Matrice di adiacenza con archi non completi (con probabilità %.2f):\n", probability);
    // printMatrix(matrix, mSize);

    saveMatrixBinary("matrix.bin", matrix, mSize);

    // Libera la memoria
    free(matrix);

    return 0;
}
