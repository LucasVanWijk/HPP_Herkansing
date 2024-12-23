#ifndef MERGE_SORT_THREADED_HPP
#define MERGE_SORT_THREADED_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <pthread.h>

using namespace std;


vector<int> merge_sort_thread(int nmbr_threads, const vector<int>& data);

// void log_time(string log_file_name, int nmbr_threads, bool verbose, vector<int> random_vector);

// vector<int> generate_random_vector(int size);

#endif // MERGE_SORT_THREADED_HPP
