#include "day02.h"

#include <stdexcept>
#include <fstream>
#include <sstream>
#include <algorithm>

// =============== PUZZLE INPUT ===============

void RedNosedReports::readPuzzleInput() {

    // clear previous 
    reports.clear();

    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // read each line and parse into report
    std::string line;
    while (std::getline(file, line)) {

        // skip empty lines
        if (line.empty())
            continue;

        std::istringstream ss(line); 

        Report rep;
        int lvl;
        while (ss >> lvl) {
            rep.levels.push_back(lvl);

        }    

        // only insert valid reports
        if (!rep.levels.empty())
            reports.push_back(rep);
    }

    file.close();
}


// =============== PART 1 ===============


bool Report::checkIfSafe() const {

    int n = levels.size();

    if (n < 2)
        return true;

    bool increasing = true;
    bool decreasing = true;
    
    for (int i = 0; i < n - 1; ++i) {
        
        int diff = levels[i+1] - levels[i];
        int absDiff = std::abs(diff);

        if (absDiff < 1 || absDiff >3)
            return false;
        
        if (diff > 0) decreasing = false;
        if (diff < 0) increasing = false;
    }

    return increasing || decreasing;
}


int RedNosedReports::solvePart1() const {

    if (reports.empty()) 
        return 0;

    int total = 0;
    for (size_t i = 0; i < reports.size(); ++i) 
        if (reports[i].checkIfSafe())
            total++;
            
    return total;        
}



// =============== PART 2 ===============


bool Report::checkIfSafeWithDampener() const {

    // skip safe reports
    if (checkIfSafe()) 
        return true;

    size_t m = levels.size();
    
    // brute-force approach, simulating all possible removals
    for (size_t i = 0; i < m; ++i) {

        // allocate copy for simulated removal
        std::vector<int> copy;
        copy.reserve(m-1);

        // copy all elements except the one being removed
        for (size_t j = 0; j < m; ++j) 
            if (j != i)
                copy.push_back(levels[j]);
        
                
        // create instance of Report to validate the copy with the removal
        Report temp{copy};
        if (temp.checkIfSafe())
            return true; // success, report is considered safe 
    }

    // after all removals, if all failed, then report is unsafe
    return false;
}

int RedNosedReports::solvePart2() const {

    if (reports.empty())
        return 0;

    int total = 0;
    for (size_t i = 0; i < reports.size(); ++i)
        if (reports[i].checkIfSafeWithDampener())
            total++;
            
    return total;        
}