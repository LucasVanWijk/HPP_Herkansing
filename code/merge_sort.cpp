#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

/**
 * @brief Merges two sorted arrays into a single sorted array.
 *
 * This function takes two sorted arrays as input and merges them into a single
 * sorted array. The resulting array contains all the elements from both input
 * arrays in sorted order.
 *
 * @param array1 The first sorted array.
 * @param array2 The second sorted array.
 * @return A vector containing all elements from array1 and array2 in sorted order.
 */
vector<int> merge_arrays(const vector<int>& array1, const vector<int>& array2) {
    vector<int> result;
    // Reserve enough space to hold all elements from both arrays.
    result.reserve(array1.size() + array2.size());
    // Initialize iterators for both arrays.
    auto it1 = array1.begin();
    auto it2 = array2.begin();

    // Loops while there are elements left in both arrays. Exits once one of the arrays is exhausted.
    while (it1 != array1.end() && it2 != array2.end()) {
        // 
        if (*it1 < *it2) {
            result.push_back(*it1);
            ++it1;
        } else {
            result.push_back(*it2);
            ++it2;
        }
    }

    result.insert(result.end(), it1, array1.end());
    result.insert(result.end(), it2, array2.end());

    return result;
}

/**
 * @brief Sorts a vector of integers using the merge sort algorithm.
 * 
 * This function recursively divides the input vector into two halves,
 * sorts each half, and then merges the sorted halves to produce the
 * final sorted vector.
 * 
 * @param data The vector of integers to be sorted.
 * @return A sorted vector of integers.
 */
vector<int> merge_sort_func(const vector<int>& data) {
    if (data.size() == 1) {
        return data;
    } else {
        int middle = data.size() / 2;
        vector<int> first(data.begin(), data.begin() + middle);
        vector<int> second(data.begin() + middle, data.end());
        return merge_arrays(merge_sort_func(first), merge_sort_func(second));
    }
}

void log_time(string log_file_name, bool verbose, vector<int> random_vector)
{
    clock_t start, end;
    clock_t clock_ticks;
    double cpu_time_used;
    
    start = clock();
    merge_sort_func(random_vector);
    end = clock();

    clock_ticks = end - start;
    cpu_time_used = (double)(clock_ticks) / CLOCKS_PER_SEC;
    // log the time used and clock ticks to the csv file
    FILE *log_file = fopen(log_file_name.c_str(), "a");
    fprintf(log_file, "%ld, %f, %ld\n", random_vector.size(), cpu_time_used, clock_ticks);
    fclose(log_file);
}

vector<int> generate_random_vector(int size)
{
    vector<int> random_vector;
    for (int i = 0; i < size; i++)
    {
        random_vector.push_back(rand());
    }
    return random_vector;
}

int main() {
    // print that the program has started
    cout << "Program has started" << endl;
    string log_file_name = "time_merge.csv";
    bool verbose = true;
    
    for(int i = 10000; i < 5000000; i += 50000)
    {
        vector<int> random_vector = generate_random_vector(i);
        log_time(log_file_name, verbose, random_vector);
    }

    return 0;
}