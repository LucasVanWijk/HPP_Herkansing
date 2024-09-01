

def distribution_pass(list_to_sort):
    # get_last_digit = lambda x: str(x)-1
    bucket = {str(k): [] for k in range(9)}
    for val in list_to_sort:
        bucket[str(val)[-1]]
    return bucket

def gathering_pass(buckets):
    new_sorted = []
    for i in range(9):
        new_sorted += buckets(str(i))
    return new_sorted

def bucket_sort():
    