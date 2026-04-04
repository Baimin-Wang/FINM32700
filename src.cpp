#include <iostream>
#include <vector>

using namespace std;

void multiply_mv_row_major(const double *matrix, int rows, int cols,
                           const double *vector, double *result) {
  for (int i = 0; i < rows; i++) {
    result[i] = 0.0;
    for (int j = 0; j < cols; j++) {
      result[i] += matrix[i * cols + j] * vector[j];
    }
  }
}

void multiply_mv_col_major(const double *matrix, int rows, int cols,
                           const double *vector, double *result) {
  for (int i = 0; i < rows; i++) {
    result[i] = 0.0;
    for (int j = 0; j < cols; j++) {
      result[i] += matrix[j * rows + i] * vector[j];
    }
  }
}

void multiply_mm_naive(const double *matrixA, int rowsA, int colsA,
                       const double *matrixB, int rowsB, int colsB,
                       double *result) {
  for (int i = 0; i < rowsA; ++i) {
    for (int j = 0; j < colsB; ++j) {
      result[i * colsB + j] = 0.0;
      for (int k = 0; k < colsA; ++k) {
        result[i * colsB + j] +=
            matrixA[i * colsA + k] * matrixB[k * colsB + j];
      }
    }
  }
}

void multiply_mm_transposed_b(const double *matrixA, int rowsA, int colsA,
                              const double *matrixB_transposed, int rowsB,
                              int colsB, double *result) {
  for (int i = 0; i < rowsA; ++i) {
    for (int j = 0; j < colsB; ++j) {
      result[i * colsB + j] = 0.0;
      for (int k = 0; k < colsA; ++k) {
        result[i * colsB + j] +=
            matrixA[i * colsA + k] * matrixB_transposed[j * rowsB + k];
      }
    }
  }
}

int main() {
  // Testing Function 1 & 2:
  int rows = 2, cols = 3;
  double *row_major_matrix = new double[6]{1, 2, 3, 4, 5, 6};
  double *col_major_matrix = new double[6]{1, 4, 2, 5, 3, 6};
  double *vec = new double[3]{7, 8, 9};

  double *result_row = new double[2];
  double *result_col = new double[2];

  multiply_mv_row_major(row_major_matrix, rows, cols, vec, result_row);
  multiply_mv_col_major(col_major_matrix, rows, cols, vec, result_col);

  cout << "Row major result: " << result_row[0] << " " << result_row[1] << endl;
  cout << "Col major result: " << result_col[0] << " " << result_col[1] << endl;
  if (result_row[0] == 50 && result_row[1]) {
    cout << "Row major result correct" << endl;
  } else {
    cout << "Row major needs some help" << endl;
  }

  if (result_col[0] == 50 && result_col[1]) {
    cout << "Col major result correct" << endl;
  } else {
    cout << "Col major needs some help" << endl;
  }
  // expected: 50 122

  delete[] row_major_matrix;
  delete[] col_major_matrix;
  delete[] vec;
  delete[] result_row;
  delete[] result_col;

  // Testing for function 3 and 4:
  // A is 2x3, B is 3x2, result is 2x2
  // Expected: A * B = [[58, 64], [139, 154]]
  int rowsA = 2, colsA = 3, rowsB = 3, colsB = 2;

  double *matA = new double[6]{1, 2, 3, 4, 5, 6};
  double *matB = new double[6]{7, 8, 9, 10, 11, 12};
  // B transposed (2x3): rows and cols of B swapped
  double *matB_T = new double[6]{7, 9, 11, 8, 10, 12};

  double *result_naive = new double[4];
  double *result_transposed = new double[4];

  multiply_mm_naive(matA, rowsA, colsA, matB, rowsB, colsB, result_naive);
  multiply_mm_transposed_b(matA, rowsA, colsA, matB_T, rowsB, colsB,
                           result_transposed);

  cout << "Naive result: " << result_naive[0] << " " << result_naive[1] << " "
       << result_naive[2] << " " << result_naive[3] << endl;
  cout << "Transposed B result: " << result_transposed[0] << " "
       << result_transposed[1] << " " << result_transposed[2] << " "
       << result_transposed[3] << endl;

  if (result_naive[0] == 58 && result_naive[1] == 64 &&
      result_naive[2] == 139 && result_naive[3] == 154)
    cout << "Naive result correct" << endl;
  else
    cout << "Naive result incorrect" << endl;

  if (result_transposed[0] == 58 && result_transposed[1] == 64 &&
      result_transposed[2] == 139 && result_transposed[3] == 154)
    cout << "Transposed B result correct" << endl;
  else
    cout << "Transposed B result incorrect" << endl;

  delete[] matA;
  delete[] matB;
  delete[] matB_T;
  delete[] result_naive;
  delete[] result_transposed;

  return 0;
}