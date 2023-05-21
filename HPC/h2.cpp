#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>

// Function to perform sequential bubble sort
void sequentialBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

// Function to perform parallel bubble sort using OpenMP
void parallelBubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    #pragma omp parallel
    {
        for (int i = 0; i < n - 1; ++i) {
            #pragma omp for
            for (int j = 0; j < n - i - 1; ++j) {
                if (arr[j] > arr[j + 1]) {
                    std::swap(arr[j], arr[j + 1]);
                }
            }
        }
    }
}

// Function to perform sequential merge sort
void merge(std::vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i) {
        L[i] = arr[l + i];
    }

    for (int j = 0; j < n2; ++j) {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }

    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void sequentialMergeSort(std::vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        sequentialMergeSort(arr, l, m);
        sequentialMergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

// Function to perform parallel merge sort using OpenMP
void parallelMergeSort(std::vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                parallelMergeSort(arr, l, m);
            }

            #pragma omp section
            {
                parallelMergeSort(arr, m + 1, r);
            }
        }

        merge(arr, l, m, r);
    }
}

// Function to generate a random vector of given size
std::vector<int> generateRandomVector(int size) {
    std::vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand() % size;
    }
    return arr;
}

int main() {
    int size = 10000;
    std::vector<int> arr = generateRandomVector(size);
    std::vector<int> arrCopy = arr;

    // Sequential bubble sort
    double startTime = omp_get_wtime();
    sequentialBubbleSort(arr);
    double endTime = omp_get_wtime();

    std::cout << "Sequential bubble sort executed in: " << endTime - startTime << " seconds.\n";

    // Parallel bubble sort
    startTime = omp_get_wtime();
    parallelBubbleSort(arrCopy);
    endTime = omp_get_wtime();

    std::cout << "Parallel bubble sort executed in: " << endTime - startTime << " seconds.\n";

    // Verify if both sorts produce the same result
    if (arr == arrCopy) {
        std::cout << "Both bubble sort algorithms produce the same result.\n";
    } else {
        std::cout << "Bubble sort algorithms produce different results!\n";
    }

    // Reset the array
    arrCopy = arr;

    // Sequential merge sort
    startTime = omp_get_wtime();
    sequentialMergeSort(arr, 0, size - 1);
    endTime = omp_get_wtime();

    std::cout << "Sequential merge sort executed in: " << endTime - startTime << " seconds.\n";

    // Parallel merge sort
    startTime = omp_get_wtime();
    parallelMergeSort(arrCopy, 0, size - 1);
    endTime = omp_get_wtime();

    std::cout << "Parallel merge sort executed in: " << endTime - startTime << " seconds.\n";

    // Verify if both sorts produce the same result
    if (arr == arrCopy) {
        std::cout << "Both merge sort algorithms produce the same result.\n";
    } else {
        std::cout << "Merge sort algorithms produce different results!\n";
    }

    return 0;
}
