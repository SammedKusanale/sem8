#include <iostream>
#include <vector>
#include <limits>
#include <numeric>
#include <omp.h>

struct ReductionResult {
    int min;
    int max;
    int sum;
    double average;

    ReductionResult()
        : min(std::numeric_limits<int>::max()),
          max(std::numeric_limits<int>::min()),
          sum(0),
          average(0.0) {}
};

ReductionResult parallelReduction(const std::vector<int>& arr) {
    ReductionResult result;

    int n = arr.size();
    #pragma omp parallel for reduction(min : result.min) reduction(max : result.max) reduction(+ : result.sum)
    for (int i = 0; i < n; ++i) {
        result.min = std::min(result.min, arr[i]);
        result.max = std::max(result.max, arr[i]);
        result.sum += arr[i];
    }

    result.average = static_cast<double>(result.sum) / n;

    return result;
}

int main() {
    int size = 10000;
    std::vector<int> arr(size);
    std::iota(arr.begin(), arr.end(), 1);

    ReductionResult result = parallelReduction(arr);

    std::cout << "Minimum: " << result.min << std::endl;
    std::cout << "Maximum: " << result.max << std::endl;
    std::cout << "Sum: " << result.sum << std::endl;
    std::cout << "Average: " << result.average << std::endl;

    return 0;
}
