#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX 3  // Adjust the matrix size as needed

int matA[MAX][MAX];
int matB[MAX][MAX];

int matSumResult[MAX][MAX];
int matDiffResult[MAX][MAX];
int matProductResult[MAX][MAX];

typedef struct {
    int row;
    int col;
} MatrixCoordinate;

void fillMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int matrix[MAX][MAX]) {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            printf("%5d", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Function to perform parallel matrix addition
void* computeSum(void* args) {
    MatrixCoordinate* coord = (MatrixCoordinate*)args;
    int row = coord->row;
    int col = coord->col;

    matSumResult[row][col] = matA[row][col] + matB[row][col];

    free(coord);
    return NULL;
}

// Function to perform parallel matrix subtraction
void* computeDiff(void* args) {
    MatrixCoordinate* coord = (MatrixCoordinate*)args;
    int row = coord->row;
    int col = coord->col;

    matDiffResult[row][col] = matA[row][col] - matB[row][col];

    free(coord);
    return NULL;
}

// Function to perform parallel matrix multiplication
void* computeProduct(void* args) {
    MatrixCoordinate* coord = (MatrixCoordinate*)args;
    int row = coord->row;
    int col = coord->col;

    int result = 0;
    for (int k = 0; k < MAX; k++) {
        result += matA[row][k] * matB[k][col];
    }
    matProductResult[row][col] = result;

    free(coord);
    return NULL;
}

int main() {
    srand(time(0));

    // 0. Get the matrix size from the command line and assign it to MAX

    // 1. Fill the matrices (matA and matB) with random values.
    fillMatrix(matA);
    fillMatrix(matB);

    // 2. Print the initial matrices.
    printf("Matrix A:\n");
    printMatrix(matA);
    printf("Matrix B:\n");
    printMatrix(matB);

    // 3. Create pthread_t objects for our threads.
    pthread_t threads[MAX * MAX];

    // 4. Create a thread for each cell of each matrix operation.
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            MatrixCoordinate* coord = (MatrixCoordinate*)malloc(sizeof(MatrixCoordinate));
            coord->row = i;
            coord->col = j;

            pthread_create(&threads[i * MAX + j], NULL, computeSum, coord);
            pthread_create(&threads[MAX * MAX + i * MAX + j], NULL, computeDiff, coord);
            pthread_create(&threads[2 * MAX * MAX + i * MAX + j], NULL, computeProduct, coord);
        }
    }

    // 5. Wait for all threads to finish.
    for (int i = 0; i < 3 * MAX * MAX; i++) {
        pthread_join(threads[i], NULL);
    }

    // 6. Print the results.
    printf("Results:\n");
    printf("Sum:\n");
    printMatrix(matSumResult);
    printf("Difference:\n");
    printMatrix(matDiffResult);
    printf("Product:\n");
    printMatrix(matProductResult);

    return 0;
}
