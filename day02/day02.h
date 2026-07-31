#pragma once
#ifndef RED_NOSED_REPORTS_H
#define RED_NOSED_REPORTS_H

#include <string>
#include <vector>

struct Report {

    std::vector<int> levels;

    // checks if report is safe using given constraints
    bool checkIfSafe() const;

    // checks if an unsafe report can be made safe by removing exactly one level
    bool checkIfSafeWithDampener() const;

};


class RedNosedReports {

public:

    RedNosedReports(const std::string& input) : puzzleInput(input) {}

    void readPuzzleInput();

    int solvePart1() const;
    int solvePart2() const;

private:

    std::string puzzleInput;
    std::vector<Report> reports;
};

#endif // RED_NOSED_REPORTS_H