#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#define high 1000.0
#define low 0.0

void csvFile(int size, double *time_spent_1, double *time_spent_2);
void initCSV();
void initMatrix(int N, double *time_spent_1, double *time_spent_2);
void fillMatrix(int N, double **A, double **B);
void commonMultiMatrix(int N, double **A, double **B, double **C);
void DGEMM_opt_1(int N, double **A, double **B, double **C);
// void printMatrix(int N, double **A, double **B, double **C);
void freeMatrix(int N, double **A, double **B, double **C);
void Matrix(int rep, int N);

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("Check!\n");
    return 0;
  }
  char *c;
  int rep = strtol(argv[2], &c, 10);
  int N = strtol(argv[1], &c, 10);

  Matrix(rep, N);
}

void Matrix(int rep, int N) {
  initCSV();
  double time_spent_1 = 0.0;
  double time_spent_2 = 0.0;
  for (int i = 0; i < rep; i++) {
    initMatrix(N, &time_spent_1, &time_spent_2);
    csvFile(N, &time_spent_1, &time_spent_2);
  }
}

void initMatrix(int N, double *time_spent_1, double *time_spent_2) {
  double **A = (double **)malloc(N * sizeof(double *));
  double **B = (double **)malloc(N * sizeof(double *));
  double **C = (double **)malloc(N * sizeof(double *));
  for (int i = 0; i < N; i++) {
    A[i] = (double *)malloc(N * sizeof(double));
    B[i] = (double *)malloc(N * sizeof(double));
    C[i] = (double *)malloc(N * sizeof(double));
  }
  fillMatrix(N, A, B);

  clock_t begin_1 = clock();
  srand(time(NULL));
  commonMultiMatrix(N, A, B, C);
  clock_t end_1 = clock();
  *time_spent_1 += (double)(end_1 - begin_1) / CLOCKS_PER_SEC;

  clock_t begin_2 = clock();
  srand(time(NULL));
  DGEMM_opt_1(N, A, B, C);
  clock_t end_2 = clock();
  *time_spent_2 += (double)(end_2 - begin_2) / CLOCKS_PER_SEC;

  // printMatrix(N, A, B, C);

  freeMatrix(N, A, B, C);
}

void fillMatrix(int N, double **A, double **B) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      A[i][j] = ((double)rand() * (high - low)) / (double)RAND_MAX + low;
      B[i][j] = ((double)rand() * (high - low)) / (double)RAND_MAX + low;
    }
}

void commonMultiMatrix(int N, double **A, double **B, double **C) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      C[i][j] = 0;
      for (int k = 0; k < N; k++) C[i][j] += A[i][k] * B[k][j];
    }
}

void DGEMM_opt_1(int N, double **A, double **B, double **C) {
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++) {
      C[i][j] = 0;
    }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      for (int k = 0; k < N; k++) C[i][k] += A[i][j] * B[j][k];
    }
  }
}

// void printMatrix(int N, double **A, double **B, double **C) {
//   printf("matrix A\n");
//   for (int i = 0; i < N; i++) {
//     for (int j = 0; j < N; j++) printf("%f ", A[i][j]);
//     printf("\n");
//   }
//   printf("\nmatrix B\n");
//   for (int i = 0; i < N; i++) {
//     for (int j = 0; j < N; j++) printf("%f ", B[i][j]);
//     printf("\n");
//   }
//   printf("\nthe result of multiplying\n");
//   for (int i = 0; i < N; i++) {
//     for (int j = 0; j < N; j++) printf("%3f ", C[i][j]);
//     printf("\n");
//   }
// }

void freeMatrix(int N, double **A, double **B, double **C) {
  for (int i = 0; i < N; i++) {
    free(A[i]);
    free(B[i]);
    free(C[i]);
  }
  free(A);
  free(B);
  free(C);
}

void initCSV() {
  FILE *file;
  file = fopen("csvFile.csv", "a");
  if (file == NULL) {
    printf("Error!\n");
    exit(0);
  }
  fprintf(file, "Size; DGEMMBLAS_Time;DGEMM_opt_1_Time\n");
  fclose(file);
}

void csvFile(int size, double *time_spent_1, double *time_spent_2) {
  FILE *file;
  file = fopen("csvFile.csv", "a");
  if (file == NULL) {
    printf("Error!\n");
    exit(0);
  }
  fprintf(file, "%d;%f;%f\n", size, *time_spent_1, *time_spent_2);
  fclose(file);
}
