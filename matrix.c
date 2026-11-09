#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define MAX_LIMIT 100
#define NUM_THREADS 10

int matA[MAX_LIMIT][MAX_LIMIT]; 
int matB[MAX_LIMIT][MAX_LIMIT]; 

int matSumResult[MAX_LIMIT][MAX_LIMIT];
int matDiffResult[MAX_LIMIT][MAX_LIMIT]; 
int matProductResult[MAX_LIMIT][MAX_LIMIT]; 

int MAX = 20; // default size matrix

void fillMatrix(int matrix[MAX_LIMIT][MAX_LIMIT]) {
  for(int i = 0; i < MAX; i++) {
    for(int j = 0; j < MAX; j++) {
      matrix[i][j] = rand() % 10 + 1;
    }
  }
}

void printMatrix(int matrix[MAX_LIMIT][MAX_LIMIT]) {
  for(int i = 0; i < MAX; i++) {
    for(int j = 0; j < MAX; j++) {
      printf("%5d", matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the sum of the
// values at the coordinates of matA and matB.
void* computeSum(void* args) { // pass in the number of the ith thread
  int thread_id = *((int*)args);
  free(args);

  int base_rows = MAX / NUM_THREADS;
  int remainder = MAX % NUM_THREADS;

  int start_row = thread_id * base_rows + (thread_id < remainder ? thread_id : remainder);
  int current_rows = base_rows + (thread_id < remainder ? 1 : 0);
  int end_row = start_row + current_rows;

  for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < MAX; j++) {
      matSumResult[i][j] = matA[i][j] + matB[i][j];
    }
  }
  pthread_exit(NULL);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the difference of the
// values at the coordinates of matA and matB.
void* computeDiff(void* args) { // pass in the number of the ith thread
  int thread_id = *((int*)args);
  free(args);

  int base_rows = MAX / NUM_THREADS;
  int remainder = MAX % NUM_THREADS;

  int start_row = thread_id * base_rows + (thread_id < remainder ? thread_id : remainder);
  int current_rows = base_rows + (thread_id < remainder ? 1 : 0);
  int end_row = start_row + current_rows;

  for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < MAX; j++) {
      matDiffResult[i][j] = matA[i][j] - matB[i][j];
    }
  }
  pthread_exit(NULL);
}

// Fetches the appropriate coordinates from the argument, and sets
// the cell of matSumResult at the coordinates to the inner product
// of matA and matB.
void* computeProduct(void* args) { // pass in the number of the ith thread
  int thread_id = *((int*)args);
  free(args);

  int base_rows = MAX / NUM_THREADS;
  int remainder = MAX % NUM_THREADS;

  int start_row = thread_id * base_rows + (thread_id < remainder ? thread_id : remainder);
  int current_rows = base_rows + (thread_id < remainder ? 1 : 0);
  int end_row = start_row + current_rows;

  for (int i = start_row; i < end_row; i++) {
    for (int j = 0; j < MAX; j++) {
      matProductResult[i][j] = 0;
      for (int k = 0; k < MAX; k++) {
        matProductResult[i][j] += matA[i][k] * matB[k][j];
      }
    }
  }
  pthread_exit(NULL);
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main(int argc, char* argv[]) {
  srand(time(0));  // Do Not Remove. Just ignore and continue below.
  
  // 0. Get the matrix size from the command line and assign it to MAX
  if (argc > 1) {
    MAX = atoi(argv[1]);
    if (MAX > MAX_LIMIT) {
      MAX = MAX_LIMIT;
    }
  }
  // 1. Fill the matrices (matA and matB) with random values.
  fillMatrix(matA);
  fillMatrix(matB);

  // 2. Print the initial matrices.
  printf("Matrix A:\n");
  printMatrix(matA);
  printf("Matrix B:\n");
  printMatrix(matB);
  
  // 3. Create pthread_t objects for our threads.
  pthread_t threads[10];

  // 4 & 5. sum
  for (int i = 0; i < 10; i++) {
    int* thread_id = malloc(sizeof(int));
    *thread_id = i;
    pthread_create(&threads[i], NULL, computeSum, thread_id);
  }
  for (int i= 0; i < 10; i++) {
    pthread_join(threads[i], NULL);
  }

  // 4 & 5. diff
  for (int i = 0; i < 10; i++) {
    int* thread_id = malloc(sizeof(int));
    *thread_id = i;
    pthread_create(&threads[i], NULL, computeDiff, thread_id);
  }
  for (int i = 0; i < 10; i++) {
    pthread_join(threads[i], NULL);
  }

  // 4 & 5. product
  for (int i = 0; i < 10; i++) {
    int* thread_id = malloc(sizeof(int));
    *thread_id = i;
    pthread_create(&threads[i], NULL, computeProduct, thread_id);
  }
  for (int i = 0; i < 10; i++) {
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