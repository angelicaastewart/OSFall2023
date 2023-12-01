#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1,0,0,0,0,0,0,0,0},
    {0,2,0,0,0,0,0,0,0},
    {0,0,3,0,0,0,0,0,0},
    {0,0,0,4,0,0,0,0,0},
    {0,0,0,0,5,0,0,0,0},
    {0,0,0,0,0,6,0,0,0},
    {0,0,0,0,0,0,7,0,0},
    {0,0,0,0,0,0,0,8,0},
    {0,0,0,0,0,0,0,0,9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

typedef struct {
    int start;
    int end;
} ThreadRange;

void printBoard(int board[SIZE][SIZE]) {
    for(int i = 0; i<SIZE; i++) {
        for(int j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void* checkRow(void* args) {
    ThreadRange* range = (ThreadRange*)args;
    int start = range->start;
    int end = range->end;

    for (int i = start; i < end; i++) {
        bool used[SIZE] = {false};
        for (int j = 0; j < SIZE; j++) {
            int current = board[i][j];
            if (current != 0) {
                if (used[current - 1]) {
                    row_check[i] = false;
                    return NULL;
                } else {
                    used[current - 1] = true;
                }
            }
        }
    }

    free(range);
    return NULL;
}

void* checkCol(void* args) {
    ThreadRange* range = (ThreadRange*)args;
    int start = range->start;
    int end = range->end;

    for (int i = start; i < end; i++) {
        bool used[SIZE] = {false};
        for (int j = 0; j < SIZE; j++) {
            int current = board[j][i];
            if (current != 0) {
                if (used[current - 1]) {
                    col_check[i] = false;
                    return NULL;
                } else {
                    used[current - 1] = true;
                }
            }
        }
    }

    free(range);
    return NULL;
}

void* checkBox(void* args) {
    ThreadRange* range = (ThreadRange*)args;
    int start = range->start;
    int end = range->end;

    for (int i = start; i < end; i++) {
        bool used[SIZE] = {false};
        int row_offset = (i / 3) * 3;
        int col_offset = (i % 3) * 3;
        for (int j = 0; j < SIZE; j++) {
            int current = board[row_offset + j / 3][col_offset + j % 3];
            if (current != 0) {
                if (used[current - 1]) {
                    box_check[i] = false;
                    return NULL;
                } else {
                    used[current - 1] = true;
                }
            }
        }
    }

    free(range);
    return NULL;
}

int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
    pthread_t row_threads[SIZE];
    pthread_t col_threads[SIZE];
    pthread_t box_threads[SIZE];

    // 3. Create a thread for each cell of each matrix operation.
    for (int i = 0; i < SIZE; i++) {
        ThreadRange* range = (ThreadRange*)malloc(sizeof(ThreadRange));
        range->start = i;
        range->end = i + 1;

        pthread_create(&row_threads[i], NULL, checkRow, range);
        pthread_create(&col_threads[i], NULL, checkCol, range);
        pthread_create(&box_threads[i], NULL, checkBox, range);
    }
    
    // 4. Wait for all threads to finish.
    for (int i = 0; i < SIZE; i++) {
        pthread_join(row_threads[i], NULL);
        pthread_join(col_threads[i], NULL);
        pthread_join(box_threads[i], NULL);
    }

    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }
    
    bool all_cols_passed = true;
    printf("Cols:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }
    
    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for (int i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }
    return 0;
}
