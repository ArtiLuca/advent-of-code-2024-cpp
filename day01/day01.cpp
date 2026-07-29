#include "day01.h"

#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <cstdlib>
#include <unordered_map>


// =============== PUZZLE INPUT ===============

void ChiefHistorian::readPuzzleInput() {

    // clear previous results
    leftList.clear();
    rightList.clear();

    // open puzzle input
    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // read in both lists 
    long long left;
    long long right;
    
    while (file >> left >> right) {
        
        leftList.push_back(left);
        rightList.push_back(right);
    }

    file.close();
}


// =============== PART 1 ===============

long long ChiefHistorian::solvePart1() {

    // create copies of lists 
    std::vector<long long> copyLeft(leftList);
    std::vector<long long> copyRight(rightList);

    // sort in increasing order
    std::sort(copyLeft.begin(), copyLeft.end());
    std::sort(copyRight.begin(), copyRight.end());

    long long total = 0;

    // iterate sorted copies and sum all distances
    for (size_t i = 0; i < copyLeft.size(); ++i) {
        total += std::abs(copyLeft[i] - copyRight[i]);
    }

    return total;
}


// =============== PART 2 ===============

long long ChiefHistorian::solvePart2(bool usingHash) {

    long long total = 0;

    // guard
    if (leftList.empty() || rightList.empty()) 
        return total;

    // using hash approach
    if (usingHash) {
        total = solvePart2_HashMap();
    }
    
     // using binary search approach
    if (!usingHash) {
        total = solvePart2_BinarySearch();
    }

    return total;
}


long long ChiefHistorian::solvePart2_BinarySearch() {

    // create copies
    std::vector<long long> leftCopy(leftList);
    std::vector<long long> rightCopy(rightList);

    // we only need the right list to be sorted
    std::sort(rightCopy.begin(), rightCopy.end());

    long long total = 0;

    for (long long left : leftCopy) {

        int firstIndex = findFirst(rightCopy, left);
        // if number exists in right list
        if (firstIndex != -1) {
            // count frequency
            int lastIndex = findLast(rightCopy, left);
            int frequency = lastIndex- firstIndex + 1;
            total = total + (left * frequency);
        }
    }

    return total;
}


int ChiefHistorian::findFirst(const std::vector<long long>& sortedRight, long long num) {

    int left = 0;
    int right = (int)sortedRight.size() - 1;
    int result = -1;

    while (left <= right) {

        int mid = (left + right) / 2;

        if (sortedRight[mid] == num) {
            result = mid;    
            // found target number, but keep searching left
            right = mid - 1;
        }
        else if (sortedRight[mid] < num) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return result;
}

int ChiefHistorian::findLast(const std::vector<long long>& sortedRight, long long num) {

    int left = 0;
    int right = (int)sortedRight.size() - 1;
    int result = -1;

    while (left <= right) {

        int mid = (left + right) / 2;

        if (sortedRight[mid] == num) {
            result = mid;
            // found target number, but keep searching right
            left = mid + 1;
        }
        else if (sortedRight[mid] < num) {
            left = mid + 1;
        }
        else {
            right = mid - 1;
        }
    }

    return result;
}

long long ChiefHistorian::solvePart2_HashMap() {

    std::unordered_map<long long, int> frequencyMap;
    
    long long total = 0;

    for (long long right : rightList) {
        // if map contains key, increment the value
        // otherwise, using [] automatically inserts key, with default value 0
        frequencyMap[right]++;
    }

    for (long long left : leftList) {
        int frequency = 0;
        if (frequencyMap.count(left) > 0) {
            frequency = frequencyMap[left];
        }
        total = total + (left * frequency);
    }

    return total;
}