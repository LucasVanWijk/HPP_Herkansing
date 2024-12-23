#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>

using namespace std;

vector<int> distribution_pass(const vector<int>& data, int j) {

    map<int, vector<int>> buckets;
    for (int i = 0; i < 10; ++i) buckets[i] = vector<int>();
    for (int x : data) {
        int index_b = (x / j) % 10;
        buckets[index_b].push_back(x);
    }

    vector<int> result_data;
    for (auto& bucket : buckets) {
        result_data.insert(result_data.end(), bucket.second.begin(), bucket.second.end());
    }

    return result_data;
}

vector<int> bucket_sort_func(vector<int> data) {
    int max_length = to_string(*max_element(data.begin(), data.end())).length();
    vector<int> pos_array, neg_array;

    if (data.empty()) return {};
    
    // Splits the array into positive and negative arrays so that the arrays can be sorted seperatly. 
    // Negative values are treated as positive and afther sorting put infront of the positive array in reverse order. 
    for (int x : data) {
        if (x >= 0) pos_array.push_back(x); 
        else neg_array.push_back(abs(x));
    }

    // Determins the number of passes needed to sort the array based on the maximum length of the numbers in the array
    for (int x = 0; x < max_length; ++x) {
        int j = pow(10, x);
        pos_array = distribution_pass(pos_array, j);
        neg_array = distribution_pass(neg_array, j);
    }

    for (int& x : neg_array) x *= -1;
    reverse(neg_array.begin(), neg_array.end());

    neg_array.insert(neg_array.end(), pos_array.begin(), pos_array.end());
    return neg_array;
}


void log_time(string log_file_name, bool verbose, vector<int> random_vector)
{
    clock_t start, end;
    clock_t clock_ticks;
    double cpu_time_used;
    
    start = clock();
    bucket_sort_func(random_vector);
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
    string log_file_name = "timer_logger.csv";
    bool verbose = true;
    
    for(int i = 1; i < 500000; i += 10000)
    {
        vector<int> random_vector = generate_random_vector(i);
        log_time(log_file_name, verbose, random_vector);
    }

    return 0;
}