#include <iostream>
#include <vector>
using namespace std;
#define CATCH_CONFIG_MAIN
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include "bucket_sort.hpp"
#include "merge_sort.hpp"
#include "merge_sort_threaded.hpp"

vector<int> generate_random_vector(int size)
{
    vector<int> random_vector;
    for (int i = 0; i < size; i++)
    {
        random_vector.push_back(rand());
    }
    return random_vector;
}

int nmbr_threads = 4;
TEST_CASE("Validate case gives exception when wrong input is given", "[bucket_sort]") {
    vector<int> random_pos = {170, 45, 75, 90, 802, 24, 2, 66};
    CHECK_FALSE(bucket_sort_func({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}) == random_pos);
    CHECK_FALSE(bucket_sort_func(random_pos) == random_pos);
}

TEST_CASE("Test 1: Random positive numbers", "[sorting]") {
    
    vector<int> random_pos = {170, 45, 75, 90, 802, 24, 2, 66};
    vector<int> random_pos_sorted = {2, 24, 45, 66, 75, 90, 170, 802};
    SECTION("Bucket Sort") {
        CHECK(bucket_sort_func(random_pos) == random_pos_sorted);
    }
    SECTION("Merge Sort") {
        CHECK(merge_sort_func(random_pos) == random_pos_sorted);
    }
    SECTION("Merge Sort Threaded") {
        CHECK(merge_sort_thread(nmbr_threads, random_pos) == random_pos_sorted);
    }
}

TEST_CASE("Test 2: Already sorted numbers", "[sorting]") {
    vector<int> already_sorted = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    SECTION("Bucket Sort") {
        CHECK(bucket_sort_func(already_sorted) == already_sorted);
    }
    SECTION("Merge Sort") {
        CHECK(merge_sort_func(already_sorted) == already_sorted);
    }
    SECTION("Merge Sort Threaded") {
        CHECK(merge_sort_thread(nmbr_threads, already_sorted) == already_sorted);
    }
}

TEST_CASE("Test 3: Already sorted numbers in reverse", "[sorting]") {
    vector<int> already_sorted = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    vector<int> already_sorted_reverse = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int nmbr_threads = 4;
    SECTION("Bucket Sort") {
        CHECK(bucket_sort_func(already_sorted_reverse) == already_sorted);
    }
    SECTION("Merge Sort") {
        CHECK(merge_sort_func(already_sorted_reverse) == already_sorted);
    }
    SECTION("Merge Sort Threaded") {
        CHECK(merge_sort_thread(nmbr_threads, already_sorted_reverse) == already_sorted);
    }
}

TEST_CASE("Test 4: All same numbers", "[sorting]") {
    vector<int> all_same = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    SECTION("Bucket Sort") {
        CHECK(bucket_sort_func(all_same) == all_same);
    }
    SECTION("Merge Sort") {
        CHECK(merge_sort_func(all_same) == all_same);
    }
    SECTION("Merge Sort Threaded") {
        CHECK(merge_sort_thread(nmbr_threads, all_same) == all_same);
    }
}

TEST_CASE("Test 5: One different number in the middle", "[sorting]") {
    vector<int> one_diff = {1, 1, 1, 1, 2, 1, 1, 1, 1, 1};
    vector<int> one_diff_sorted = {1, 1, 1, 1, 1, 1, 1, 1, 1, 2};
    SECTION("Bucket Sort") {
        CHECK(bucket_sort_func(one_diff) == one_diff_sorted);
    }
    SECTION("Merge Sort") {
        CHECK(merge_sort_func(one_diff) == one_diff_sorted);
    }
    SECTION("Merge Sort Threaded") {
        CHECK(merge_sort_thread(nmbr_threads, one_diff) == one_diff_sorted);
    }
}

TEST_CASE("Test 7: One element", "[sorting]") {
    vector<int> one_element = {1};
    SECTION("Bucket Sort") {
        CHECK(bucket_sort_func(one_element) == one_element);
    }
    SECTION("Merge Sort") {
        CHECK(merge_sort_func(one_element) == one_element);
    }
    SECTION("Merge Sort Threaded") {
        CHECK(merge_sort_thread(nmbr_threads, one_element) == one_element);
    }
}

TEST_CASE("Test 8: Numbers with negatives", "[sorting]") {
    vector<int> with_negatives = {1, 2, 5, 4, 4, 0, -1, -2, -3, -4, -5};
    vector<int> with_negatives_sorted = {-5, -4, -3, -2, -1, 0, 1, 2, 4, 4, 5};

    SECTION("Bucket Sort") {
        CHECK(bucket_sort_func(with_negatives) == with_negatives_sorted);
    }
    SECTION("Merge Sort") {
        CHECK(merge_sort_func(with_negatives) == with_negatives_sorted);
    }
    SECTION("Merge Sort Threaded") {
        CHECK(merge_sort_thread(nmbr_threads, with_negatives) == with_negatives_sorted);
    }
}
// TEST_CASE("Benchmarking bucket_sort algorithms", "[bucket_sort_benchmark]") {
//     // BENCHMARK_ADVANCED("Bucket sort")(Catch::Benchmark::Chronometer meter) {
//     //     for(int i = 1; i < 500000; i += 10000)
//     //         {
//     //             vector<int> random_vector = generate_random_vector(i);
//     //             meter.measure([&random_vector](i) {
//     //                 return bucket_sort_func(random_vector);
//     //             });
//     //         }
//     // }
//             for(int i = 1; i < 500000; i += 10000)
//             {
//                 vector<int> random_vector = generate_random_vector(i);
//                 meter.measure([&random_vector](i) {
//                     return bucket_sort_func(random_vector);
//                 });
//             }
//     BENCHMARK("indexed", i){ return long_computation(i); };
// }
