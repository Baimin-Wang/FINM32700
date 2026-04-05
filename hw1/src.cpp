/*
 * Matrix Sum Optimization
 *
 * Optimizations applied in sumMatrixOptimized:
 *
 * 1. INLINING: getElement() and add() have been inlined. Each call in the
 *    nested loop (SIZE*SIZE = ~16M iterations) incurs stack frame overhead.
 *    Inlining eliminates that entirely.
 *
 * 2. CACHE LOCALITY: std::vector<std::vector<int>> stores each row as a
 *    separate heap allocation. We access each row's contiguous data block
 *    directly via row.data(), iterating sequentially without index arithmetic.
 *    This keeps the access pattern linear within each row's cache lines.
 *
 * 3. POINTER ARITHMETIC: Instead of matrix[i][j] (two pointer dereferences +
 *    index arithmetic each iteration), we cache a row pointer once per outer
 *    loop iteration and advance it with ++ptr. Eliminates repeated index
 *    multiply-add operations.
 *
 * 4. LOOP UNROLLING (4x): Each inner iteration processes 4 elements, reducing
 *    loop control overhead (increment + branch) by 4x. The leftover elements
 *    are handled in a scalar tail loop.
 */

#include <iostream>
#include <vector>
#include <chrono>
#include <random>

const int SIZE = 4096;

// Basic function to access matrix elements
int getElement(const std::vector<std::vector<int>>& matrix, int row, int col) {
    return matrix[row][col];
}

// Basic function to add two integers
int add(int a, int b) {
    return a + b;
}

// Unoptimized summation function
long long sumMatrixBasic(const std::vector<std::vector<int>>& matrix) {
    long long sum = 0;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            sum = add(sum, getElement(matrix, i, j));
        }
    }
    return sum;
}

// Optimized summation function
long long sumMatrixOptimized(const std::vector<std::vector<int>>& matrix) {
    long long sum = 0;

    for (int i = 0; i < SIZE; ++i) {
        // Cache the raw row pointer once — avoids repeated vector overhead
        const int* ptr = matrix[i].data();
        const int* end4 = ptr + (SIZE & ~3); // largest multiple of 4 <= SIZE

        // 4x unrolled inner loop with pointer arithmetic
        for (; ptr < end4; ptr += 4) {
            sum += ptr[0];
            sum += ptr[1];
            sum += ptr[2];
            sum += ptr[3];
        }
        // Tail: handle remaining 0-3 elements per row
        const int* row_end = matrix[i].data() + SIZE;
        for (; ptr < row_end; ++ptr)
            sum += *ptr;
    }

    return sum;
}

int main() {
    // Generate a large random matrix
    std::vector<std::vector<int>> matrix(SIZE, std::vector<int>(SIZE));
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(-100, 100);
    for (int i = 0; i < SIZE; ++i)
        for (int j = 0; j < SIZE; ++j)
            matrix[i][j] = distrib(gen);

    // Basic
    auto start = std::chrono::high_resolution_clock::now();
    long long sum = sumMatrixBasic(matrix);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Basic Sum:        " << sum << std::endl;
    std::cout << "Basic Time:       " << duration.count() << " ms" << std::endl;

    // Optimized
    auto start_optimized = std::chrono::high_resolution_clock::now();
    long long optimized_sum = sumMatrixOptimized(matrix);
    auto end_optimized = std::chrono::high_resolution_clock::now();
    auto duration_optimized = std::chrono::duration_cast<std::chrono::milliseconds>(end_optimized - start_optimized);
    std::cout << "Optimized Sum:    " << optimized_sum << std::endl;
    std::cout << "Optimized Time:   " << duration_optimized.count() << " ms" << std::endl;

    // Correctness check
    if (sum == optimized_sum)
        std::cout << "Correctness: PASS" << std::endl;
    else
        std::cout << "Correctness: FAIL" << std::endl;

    return 0;
}
