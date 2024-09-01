use std::vec;

fn distribution_pass(list_to_sort: Vec<i32>, index: u32) -> Vec<Vec<i32>> {
    let mut bucket: Vec<Vec<i32>> = vec![Vec::new(); 10];

    for val in list_to_sort {
        let base: i32 = 10;
        let key: i32 = val / base.pow(index) % 10;
        //let key = key as usize;
        bucket[key as usize].push(val);
    }
    bucket
}

fn gathering_pass(buckets: Vec<Vec<i32>>) -> Vec<i32> {
    let mut sorted_list: Vec<i32> = Vec::new();
    for bucket in buckets {
        sorted_list.extend(bucket);
    }
    sorted_list
}

pub fn sorter(list_to_sort:Vec<i32>) -> Vec<i32> {
    let mut list_to_sort: Vec<i32> = list_to_sort.clone();  
    let mut orderd_list:Vec<i32> = Vec::new();
    let mut is_sorted: bool = false;
    let base: i32 = 10;
    let mut i: u32 = 0;
    while is_sorted == false {
        let buckets: Vec<Vec<i32>> = distribution_pass(list_to_sort, i);
        let sorted_list: Vec<i32> = gathering_pass(buckets);
        
        list_to_sort = Vec::new();
        let factor: i32 = base.pow((i + 1) as u32);
        for val in sorted_list{
            if val < factor{
                orderd_list.push(val)
            }
            else{
                list_to_sort.push(val)
            }
        }
        //prints orderd_list
        is_sorted = if list_to_sort.len() == 0 {true} else {false};
        i = i + 1;
    }
    orderd_list
}