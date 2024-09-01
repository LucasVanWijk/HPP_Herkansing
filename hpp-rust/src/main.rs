mod bucket_sort;

use std::time::Instant;
use rand::Rng;
use std::time;
use async_std::task;
use csv::Writer;


/// Receives three numbers: the maximum value of the list, the minimum value of the list, and the length of the list.
/// Returns a vector containing the generated list with random values within the specified range.
fn generate_random_list(max: i32, min: i32, length: i32) -> Vec<i32> {
    let mut rng = rand::thread_rng();
    let mut list: Vec<i32> = Vec::new();
    for _ in 0..length {
        list.push(rng.gen_range(min..max));
    }
    list
}


/// Receives a list and a function to sort the list.
/// Returns the sorted list and the time it took to sort the list.
fn time_sort(list_to_sort: Vec<i32>, sorter: fn(Vec<i32>) -> Vec<i32>) -> (Vec<i32>, time::Duration) {
    let start = Instant::now();
    let sorted_list = sorter(list_to_sort);
    let duration = start.elapsed();
    (sorted_list, duration)
}



/// Recieves a number of iterations it needs to run, the maximum value of the list, the minimum value of the list and a string representing a file name.
/// Generates a list of 10 values, times how long it takes to sort the list, and writes the time to a file and test parameters to a file.
fn run_test(iterations: i32, max: i32, min: i32) -> Vec<(i32, i32, i32, u128)> {
    // create a file to write the test parameters if it doesn't exist
    
    let mut results: Vec<(i32, i32, i32, u128)> = vec![];
    for i in 10..iterations {
        let base: i32 = 2;
        let max_length: i32 = base.pow(i as u32);
        let list_to_sort = generate_random_list(max, min, max_length);
        let (_sorted_list, duration) = time_sort(list_to_sort, bucket_sort::sorter);
        //writes the lenght of the list, the minimum value, the maximum value and the time it took to sort the list to a list
        results.push((max_length, min, max, duration.as_nanos()));
        
    }
    results
}

fn main() -> Result<(), csv::Error> {
    let file_name = "bucket_sort.csv";
    let mut wtr = Writer::from_path(file_name)?;

    let iterations = 20;
    let max = 1000;
    let min = 0;
    
    let results: Vec<(i32, i32, i32, u128)> = run_test(iterations, max, min);

    for record in results {
        wtr.write_record(&[record.0.to_string(), record.1.to_string(), record.2.to_string(), record.3.to_string()])?;
    }

    Ok(())
}
