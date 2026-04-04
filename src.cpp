#include <iostream>
#include <vector>

using namespace std;

void multiply_mv_row_major(const double* matrix, int rows, int cols, const double* vector, double* result){
    for (int i = 0; i < rows; i ++){
        result[i] = 0.0;
        for (int j = 0; j < cols; j ++){
            result[i] += matrix[i * cols + j] * vector[j];
        } 
    }
}

void multiply_mv_col_major(const double* matrix, int rows, int cols, const double* vector, double* result){
    for (int i = 0; i < rows; i ++){
        result[i] = 0.0;
        for (int j = 0; j < cols; j ++){
            result[i] += matrix[j * rows + i] * vector[j];
        } 
    }
}

void multiply_mm_naive(const double* matrixA, int rowsA, int colsA, const double* matrixB, int rowsB, int colsB, double* result);

void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA, const double* matrixB_transposed, int rowsB, int colsB, double* result);

int main(){
    // Testing Function 1 & 2:
    int rows = 2, cols = 3;
    double* row_major_matrix = new double[6]{1, 2, 3, 4, 5, 6};
    double* col_major_matrix = new double[6]{1, 4, 2, 5, 3, 6};
    double* vec = new double[3]{7, 8, 9};

    double* result_row = new double[2];
    double* result_col = new double[2];

    multiply_mv_row_major(row_major_matrix, rows, cols, vec, result_row);
    multiply_mv_col_major(col_major_matrix, rows, cols, vec, result_col);

    cout << "Row major result: " << result_row[0] << " " << result_row[1] << endl;
    cout << "Col major result: " << result_col[0] << " " << result_col[1] << endl;
    if (result_row[0] == 50 && result_row[1]){
        cout << "Row major result correct" << endl;
    }
    else{
        cout <<"Row major needs some help" << endl;
    }

    if (result_col[0] == 50 && result_col[1]){
        cout << "Col major result correct" << endl;
    }
    else{
        cout <<"Col major needs some help" << endl;
    }
    // expected: 50 122

    delete[] row_major_matrix;
    delete[] col_major_matrix;
    delete[] vec;
    delete[] result_row;
    delete[] result_col;

    return 0;
}