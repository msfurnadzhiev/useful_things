#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include "Timer.h"

size_t INITIAL_SIZE = 32768;
size_t DIFFICULTY_FACTOR = 100000;
size_t RANGE = 256;
size_t LIMIT = 128;

clock_t test_branch_prediction_with_random_numbers() {
    int* A = new int[INITIAL_SIZE];

    for (size_t i = 0; i < INITIAL_SIZE; i++) {
        A[i] = std::rand() % RANGE;
    }
    Timer t;
    unsigned long long sum = 0;

    for (size_t j = 0; j < DIFFICULTY_FACTOR; j++) {

        for (size_t i=0; i < INITIAL_SIZE; i++) {
            if (A[i] >= LIMIT) {
                sum += A[i];
            }
        }
    }

    std::cout << __FUNCTION__  << " | Sum (" << sum << ") ";
    return t.elapsed_time();
}
clock_t test_branch_prediction_with_sorted_numbers() {

    int* A = new int[INITIAL_SIZE];

    for (size_t i = 0; i < INITIAL_SIZE; i++) {
        A[i] = std::rand() % RANGE;
    }
    std::sort(A,A+INITIAL_SIZE);

    Timer t;
    unsigned long long sum = 0;

    for (size_t j = 0; j < DIFFICULTY_FACTOR; j++) {
        for (size_t i=0; i < INITIAL_SIZE; i++) {
            if (A[i] >= LIMIT) {
                sum += A[i];
            }
        }
    }

    std::cout << __FUNCTION__  << " | Sum (" << sum << ") ";
    return t.elapsed_time();
}
clock_t test_without_branch_prediction_with_random_numbers() {
    int* A = new int[INITIAL_SIZE];

    for (size_t i = 0; i < INITIAL_SIZE; i++) {
        A[i] = std::rand() % RANGE;
    }
    Timer t;
    unsigned long sum = 0;

    for (size_t j = 0; j < DIFFICULTY_FACTOR; j++) {

        for (size_t i=0; i < INITIAL_SIZE; i++) {
            //if a < 128 then c is -1 else c is 0
            int c = (A[i] - LIMIT) >> 31;
            //with ~c convert condition (if c == 0 then c = -1 else c = 0
            //two case: if c == 0 then (0 & A[i]) = 0 else (-1 & A[i]) = A[i]
            sum += ~c & A[i];
        }
    }

    std::cout << __FUNCTION__  << " | Sum (" << sum << ") ";
    return t.elapsed_time();
}
clock_t test_without_branch_prediction_with_sorted_numbers() {
    int* A = new int[INITIAL_SIZE];

    for (size_t i = 0; i < INITIAL_SIZE; i++) {
        A[i] = std::rand() % RANGE;
    }
    std::sort(A,A + INITIAL_SIZE);

    Timer t;
    unsigned long sum = 0;

    for (size_t j = 0; j < DIFFICULTY_FACTOR; j++) {

        for (size_t i=0; i < INITIAL_SIZE; i++) {
            //if a < 128 then c is -1 else c is 0
            int c = (A[i] - LIMIT) >> 31;
            //with ~c convert condition (if c == 0 then c = -1 else c = 0
            //two case: if c == 0 then (0 & A[i]) = 0 else (-1 & A[i]) = A[i]
            sum += ~c & A[i];
        }
    }

    std::cout << __FUNCTION__  << " | Sum (" << sum << ") ";
    return t.elapsed_time();
}

using test_function_ptr = clock_t(*)();

int main() {
    std::vector<test_function_ptr> F = {
            test_branch_prediction_with_random_numbers,
            test_branch_prediction_with_sorted_numbers,
            test_without_branch_prediction_with_random_numbers,
            test_without_branch_prediction_with_sorted_numbers
    };

    for(test_function_ptr f : F) {
        clock_t time = f();
        std::cout << " | Time" << ": " << static_cast<double>(time) / CLOCKS_PER_SEC << " seconds\n\n";
    }
}
