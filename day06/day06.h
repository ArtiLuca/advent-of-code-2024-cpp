#pragma once

#include <string>
#include <vector>
#include <utility>
#include <set>


/* Tracks guard's movement during patrol simulation */
struct Guard {

    int row;
    int col;
    int dir;

    // rotates guard 90 degrees right in place
    void turnRight();
    // returns coordinates of guard's next position 
    std::pair<int,int> getNextPosition() const;
};


class Patrol {

public:

    Patrol(const std::string& input) : puzzleInput(input) {}

    // reads puzzle input and finds guard's starting position 
    void readPuzzleInput();

    // returns number of distinct positions visited by guard during patrol 
    int solvePart1();

    // runs brute-force simulations to find obstruction that causes loop  
    int solvePart2();

private:

    // true if guard steps out of mapped area (out-of-bounds)
    bool isOutsideMappedArea(int r, int c) const;

   // Runs a fresh simulation with the current map layout.
   // Returns true if an infinite loop is caught, false if the guard exits.
   bool simulateLoop(std::vector<char>& stateVisited); 

    std::string puzzleInput;
    std::vector<std::string> map;
    Guard initialGuardState;
};