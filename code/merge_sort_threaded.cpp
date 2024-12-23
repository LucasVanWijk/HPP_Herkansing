#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#include <pthread.h>

using namespace std;

struct thread_args {
    const vector<int>* data;
    int start;
    int end;
    vector<int>* sorted_subvector;
};

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

void* merge_sort_thread_wrapper(void* args) {
    thread_args* t_args = static_cast<thread_args*>(args);
    int start = t_args->start;
    int end = t_args->end;
    vector<int> subvector(t_args->data->begin() + start, t_args->data->begin() + end);
    *t_args->sorted_subvector = merge_sort_func(subvector);
    return NULL;
}

vector<int> merge_sort_thread(int nmbr_threads, const vector<int>& data) {
    int elements_per_thread = ceil(static_cast<double>(data.size()) / nmbr_threads);
    vector<pthread_t> threads(nmbr_threads);
    vector<vector<int>> sorted_subvectors(nmbr_threads);
    vector<thread_args> thread_args(nmbr_threads);

    for (int i = 0; i < nmbr_threads; ++i) {
        int start = i * elements_per_thread;
        int end = min((i + 1) * elements_per_thread, static_cast<int>(data.size()));
        thread_args[i] = {&data, start, end, &sorted_subvectors[i]};
        pthread_create(&threads[i], NULL, merge_sort_thread_wrapper, &thread_args[i]);
    }

    for (int i = 0; i < nmbr_threads; ++i) {
        pthread_join(threads[i], NULL);
    }

    vector<int> sorted_vector;
    for (const auto& subvector : sorted_subvectors) {
        sorted_vector = merge_arrays(sorted_vector, subvector);
    }

    return sorted_vector;
}

void log_time(string log_file_name, int nmbr_threads, bool verbose, vector<int> random_vector)
{
    clock_t start, end;
    clock_t clock_ticks;
    double cpu_time_used;
    
    start = clock();
    merge_sort_thread(nmbr_threads, random_vector);
    end = clock();

    clock_ticks = end - start;
    cpu_time_used = (double)(clock_ticks) / CLOCKS_PER_SEC;
    // log the time used and clock ticks to the csv file
    FILE *log_file = fopen(log_file_name.c_str(), "a");
    fprintf(log_file, "%d, %ld, %f, %ld\n",nmbr_threads, random_vector.size(), cpu_time_used, clock_ticks);
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
    string log_file_name = "time_merge_thread.csv";
    bool verbose = true;
    for(int nmbr_threads = 1; nmbr_threads < 12; nmbr_threads += 2){
        for(int i = 10000; i < 1000000; i += 100000)
        {
            vector<int> random_vector = generate_random_vector(i);
            log_time(log_file_name, nmbr_threads, verbose, random_vector);
        }
    }

    return 0;
}