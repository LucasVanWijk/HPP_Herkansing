import random

def bucket_sort(list_to_sort):
    # This algorithm is divided in a first iteration and a second to n iteration. 
    # This is because certain operations are not necessary to make in the first iteration.
    # And others not necessary in the second to n iteration.
    # So to save on computation time the algorithm has been split up it to two functions.
    # Instead of one single generic function. 

    def for_first_iteration():
        buckets = {'0':[], '1': [], '2': [], '3': [], '4': [], '5': [], '6': [], '7': [], '8': [], '9': []}
        
        #Goes trough all the values in the list and places them into buckets. So that they can be sorted
        for item in list_to_sort:
            string_nummer = str(item)
            buckets[string_nummer[-1]].append(string_nummer)
        
        # Deletes the list and then places all the elements in the buckets back into the list.
        # Now having sorted all numbers on there first numerical digit
        del list_to_sort[:]
        for bucket in buckets:
            numbers_to_sort = buckets[bucket]
            for element in numbers_to_sort:
                list_to_sort.append(element)
        return list_to_sort
    

    def second_to_n_iteration():
        numerical_digit = 1        
        while True:
            buckets = {'0':[], '1': [], '2': [], '3': [], '4': [], '5': [], '6': [], '7': [], '8': [], '9': []}
            index_in_list_to_sort = 0
            while index_in_list_to_sort < len(list_to_sort):
                item = list_to_sort[index_in_list_to_sort]
                
                if len(item) > numerical_digit:
                    buckets[item[-1 -numerical_digit]].append(item)
                    del list_to_sort[index_in_list_to_sort]
                else:
                    # Index should only increase if a item is not removed.
                    # If a item is removed the list get's shorter.
                    # And The next number will now be at the old index of the removed number.
                    # Therfore is should not increase its index because it would than skip a number.
                    index_in_list_to_sort += 1 

            # If the buckets are empty that means,
            # there are no more numbers that need to be sorted acording to its numerical digit
            # So it should break out of the loop and return the sorted list.
            if buckets == {'0':[], '1': [], '2': [], '3': [], '4': [], '5': [], '6': [], '7': [], '8': [], '9': []}:
                break
            
            for bucket in buckets:
                numbers_to_sort = buckets[bucket]
                for element in numbers_to_sort:
                    list_to_sort.append(element)
            
            numerical_digit += 1
        
        return list_to_sort
    
    list_to_sort = for_first_iteration()
    list_to_sort = second_to_n_iteration()

    return list_to_sort