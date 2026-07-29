# Advent of Code 2024 - Day 1: Historian Hysteria

## Part 1
In Part 1, we are given two lists of location IDs. Our goal is to repeatedly find the smallest-value IDs from both lists and calculate their distance (i.e., how far apart they are). We then must sum up all of these distances to get the solution for Part 1.

### Idea
Since we are extracting the smallest-value pairs of IDs from both lists at each step, we can first sort both lists in increasing order. That way, we can perform a single linear scan that calculates the distance for each pair using the absolute value of their difference, while summing them up to get Part 1's solution. For the actual implementation, we simply use `std::sort` for sorting.

### Pseudocode
Assuming `L[0...N]` and `R[0...N]` contain our lists of numbers read from the input:

```text
solvePart1(L, R)
    sort L in increasing order
    sort R in increasing order
    total = 0
    for i = 0 to N
        total = total + |L[i] - R[i]|

    return total    
```

### Complexity
The complexity is determined by the initial sorting of both lists. Being a comparison sort, it is strictly bounded by $\Theta(N \log N)$. We then scan the sorted lists exactly $N$ times, performing constant work at each step. Therefore, the time complexity is $T(N) = O(N \log N) + O(N \log N) + O(N) = \Theta(N \log N)$. The auxiliary space complexity is $\Theta(N)$, since each list of numbers read from the input takes up $\Theta(N)$ space.

---

## Part 2    
In Part 2, we learn that we are no longer interested in the minimum values, but instead want to find the similarity scores. A similarity score is found by multiplying a number in the left list by the number of times it appears in the right list. We want to do this for every number in order to calculate the total similarity score.

### Idea
In Part 2, we can explore two approaches for solving the problem. The first approach uses the sorted lists from Part 1 and implements an iterative binary search, while the second approach completely ignores the sorting phase and uses a hash map. I decided to look at both approaches, since the binary search approach is the one I am most familiar with, while the hashing approach uses concepts that are "newer" to me and also proves to be more efficient.

### (1) Binary Search Approach  

For this approach, we need both lists to be sorted in increasing order so that any duplicate numbers in the right list are grouped together in a contiguous block. Then, for each number in the left list, we can find its exact frequency in the right list by implementing a slightly modified binary search to find the boundaries of that block. To do this, we use two helpers: one for finding the position of the very first occurrence of the target number on the left, and another for finding the very last position of the target number on the right. We can then subtract the first position from the last position and add 1 to find the exact frequency.

### Pseudocode (Binary Search)

```text
findFirst(R, num)
    left = 0
    right = R.length - 1
    result = -1

    while left <= right
        mid = (left + right) / 2

        if R[mid] == num
            result = mid
            // found target number, but keep searching left
            right = mid - 1    
        else if R[mid] < num    
            left = mid + 1
        else
            right = mid - 1   

    return result

findLast(R, num)
    left = 0
    right = R.length - 1
    result = -1

    while left <= right
        mid = (left + right) / 2

        if R[mid] == num
            result = mid
            // found target number, but keep searching right
            left = mid + 1    
        else if R[mid] < num    
            left = mid + 1
        else
            right = mid - 1   

    return result        

solvePart2_BinarySearch(L, R)
    total = 0
    for num in L
        firstIndex = findFirst(R, num)

        // if number exists in right list
        if firstIndex != -1
            lastIndex = findLast(R, num)
            frequency = lastIndex - firstIndex + 1
            total = total + (num * frequency)

    return total        
```

### (2) Hash Map Approach  

For this approach, we store key-value pairs (for example, using `std::unordered_map`) and can completely ignore the sorting phase. Instead of sorting, we do a single pass over the right list, using every number we find as the "key" and incrementing its "value" (the count) by 1. Once we have built the full map, we can use it as a frequency dictionary to look up each number in the left list. If a number in the left list exists in the dictionary, we multiply it by its stored frequency and add it to the total.

### Pseudocode (Hash Map)

```text
solvePart2_HashMap(L, R)
    create new frequencyMap as empty Hash Map
    total = 0

    // build frequency dictionary from right list
    for num in R
        if num exists in frequencyMap
            frequencyMap[num] = frequencyMap[num] + 1
        else
            frequencyMap[num] = 1

    // calculate similarity scores using left list
    for num in L
        frequency = 0
        if num exists in frequencyMap
            frequency = frequencyMap[num]      

        total = total + (num * frequency)

    return total              
```

### Complexity
**If we use a binary search approach:**
Sorting both arrays initially takes $\Theta(N \log N)$ time. Then, for each of the $N$ elements in the left list, we perform two binary searches. Since each binary search takes $O(\log N)$ time, the scanning phase takes $O(N \log N)$ time. This yields an overall time complexity of $\Theta(N \log N)$. Assuming we copy the lists to avoid mutating the original input, the space complexity is $O(N)$.

**If we use a hash map approach:**
Building the frequency dictionary requires iterating through the right list of size $N$ once. Inserting into a hash map takes $O(1)$ time on average, making this step $O(N)$. Calculating the scores requires iterating through the left list of size $N$ once, with hash map lookups also taking $O(1)$ time on average. This brings the overall time complexity down to strictly $O(N)$. However, because we must allocate a new data structure to store the unique numbers and their counts, the space complexity increases to $O(N)$ in the worst-case scenario (e.g., if all numbers in the right list are unique).
