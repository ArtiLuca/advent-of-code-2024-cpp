#include "day05.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <algorithm>


// =============== PUZZLE INPUT ===============

void PrintQueue::readPuzzleInput() {

    // clear previous results
    rules.clear();
    updates.clear();

    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // read page-ordering rules first
    std::string line;
    bool readingRules = true;

    while (std::getline(file, line)) {

        // if we encounter a blank lines we have read all rules and can move to update sequences
        if (line.empty()) {
            readingRules = false;
            continue;;
        }

        if (readingRules) {
            
            // read a single X|Y page ordering rule
            std::stringstream ss(line);
            int x,y;
            char mid;

            if (ss >> x >> mid >> y && mid == '|') {

                Rule currentRule;
                currentRule.before = x;
                currentRule.after = y;
                
                // map rule to our associative rule map 
                rules[currentRule.before].insert(currentRule.after);
            }
        }
        else {  // otherwise we are reading update sequences

            std::stringstream ss(line);
            std::string number;
            Update currentUpdate;

            while (std::getline(ss, number, ',')) {
                currentUpdate.pages.push_back(std::stoi(number));
            }

            // store successfully parsed update sequence
            updates.push_back(currentUpdate);
        }
    }

    file.close();
}

// =============== PART 1 ===============

int Update::getMiddlePoint() const { return pages[pages.size() / 2]; }

bool PrintQueue::isCorrectlyOrdered(const Update& update) const {

    // guard
    if (update.pages.empty())
        return false;

    size_t n = update.pages.size();

    // inspect all pairs for possible page ordering violation
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++ j) {

            // check currrent page with its subsequent page
            int currentPage = update.pages[i];
            int subsequentPage = update.pages[j];

            // if our rule map contains the subsequentPage, 
            // and it contains the currentPage as a page that must be printed after,
            if (rules.count(subsequentPage) && rules.at(subsequentPage).count(currentPage)) {
                // then we found a violation, so the entire update is not correctly ordered
                return false;
            }
        }
    }

    // if no violations found, update is correctly ordered
    return true;
}

int PrintQueue::solvePart1() const {
    
    int sum = 0;
    for (const auto& update : updates) {
        // only consider correctly-ordered updates
        if (isCorrectlyOrdered(update))
            sum += update.getMiddlePoint();
    }

    return sum;
}


// =============== PART 2 ===============

int PrintQueue::solvePart2() const {

    // assuming input was already parsed
    int sum = 0;

    for (const auto& update: updates) {

        // only considere incorrectly-ordered update sequences
        if (!isCorrectlyOrdered(update)) {

            // copy update sequence
            std::vector<int> updateCopy = update.pages;

            // custom lambda comparator
            // [&] capturs 'this->rules' by reference in order to read the rules
            std::sort(updateCopy.begin(), updateCopy.end(), [&](int a, int b) {
                // if rule exists stating 'b' must come after 'a', return true
                if (rules.count(a) && rules.at(a).count(b)) 
                    return true;
                // otherwise, 'a' has no strict precedence over 'b'    
                return false;
            });

            // calculate middle-point and add to total
            int middleIndex = updateCopy.size() / 2;
            sum += updateCopy[middleIndex];
        }
    }

    return sum;
}