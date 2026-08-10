#include "day06.h"

#include <fstream>
#include <stdexcept>
#include <set>
#include <vector>

// =============== CONSTANTS ===============

const char DIR_UP = '^';
const char DIR_RIGHT = '>';
const char DIR_DOWN = 'v';
const char DIR_LEFT = '<';
const char OBSTRUCTION = '#';
const char EMPTY = '.';

const int dr[] = {-1, 0, 1, 0};
const int dc[] = {0, 1, 0, -1};


// =============== GUARD STRUCT ===============

void Guard::turnRight() {
    dir = (dir + 1) % 4;
}

std::pair<int,int> Guard::getNextPosition() const {
    return {row + dr[dir], col + dc[dir]};
}


// =============== PATROL CLASS ===============

bool Patrol::isOutsideMappedArea(int r, int c) const { 
    return (r < 0 || r >= (int)map.size() || c < 0 || c >= (int)map[0].size());
}

// =============== PUZZLE INPUT ===============

void Patrol::readPuzzleInput() {

    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // read 2D grid from input
    std::string line;
    while (std::getline(file,line)) {

        // only use non-empty lines
        if (!line.empty())
            map.push_back(line);
    }

    int rows = map.size();
    int cols = map[0].size();

    // locate guard's starting position
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            char cell = map[r][c];
            if (cell == DIR_UP || cell == DIR_RIGHT || cell == DIR_DOWN || cell == DIR_LEFT) {
              
                initialGuardState.row = r;
                initialGuardState.col = c;
                
                // convention used for guard's direction: 
                // 0=Up, 1=Right, 2=Down, 3=Left 
                
                if (cell == DIR_UP) initialGuardState.dir = 0;
                if (cell == DIR_RIGHT) initialGuardState.dir = 1;
                if (cell == DIR_DOWN) initialGuardState.dir = 2;
                if (cell == DIR_LEFT) initialGuardState.dir = 3;

                map[r][c] = EMPTY;
                return;
            }
        }
    }
    
}


// =============== PART 1 ===============

int Patrol::solvePart1() {

    // guard
    if (map.empty()) 
        return -1;

    // create a working copy from the guard's stored initial position    
    Guard guard = initialGuardState;
    std::set<std::pair<int,int>> visited;

    // insert guard's initial position
    visited.insert({guard.row, guard.col});

    // run simulation
    while (true) {

        // find next position in guard's direction
        std::pair<int,int> next = guard.getNextPosition();

        // check if next position is outside mapped area
        if (isOutsideMappedArea(next.first, next.second)) {
            // if it is, we stop the simulation
            break;
        }

        // otherwise, if there is an obstruction the guard turns right 90 degrees in place
        if (map[next.first][next.second] == OBSTRUCTION) {
            guard.turnRight();
        }

        // otherwise, the guard moves to the next position
        else {
            // move guard foward
            guard.row = next.first;
            guard.col= next.second;
            visited.insert({guard.row, guard.col});
        }
    }

    return (int) visited.size();
}


// =============== PART 2 ===============

bool Patrol::simulateLoop(std::vector<char>& stateVisited) {

    int cols = map[0].size();
    Guard guard = initialGuardState;

    // Helper lambda to calculate 1D index from 3D coordinates
    auto getIndex = [&](int r, int c, int d) {
        return (r * cols * 4) + (c * 4) + d;
    };

    // Log the starting state
    stateVisited[getIndex(guard.row, guard.col, guard.dir)] = 1;

    while (true) {
        std::pair<int,int> next = guard.getNextPosition();

        if (isOutsideMappedArea(next.first, next.second)) {
            return false;
        }

        if (map[next.first][next.second] == OBSTRUCTION) {
            guard.turnRight();
        }
        else {
            guard.row = next.first;
            guard.col = next.second;
        }

        int idx = getIndex(guard.row, guard.col, guard.dir);
        
        // Loop verification
        if (stateVisited[idx]) {
            return true;
        }

        // Log the new state
        stateVisited[idx] = 1;
    }
}


int Patrol::solvePart2() {

    // simulate Part 1 logic to get candidate positions (patrol route)
    Guard guard = initialGuardState;
    std::set<std::pair<int,int>> patrolRoute;

    while (true) {
        std::pair<int,int> next = guard.getNextPosition();
        
        if (isOutsideMappedArea(next.first, next.second))
            break;

        if (map[next.first][next.second] == OBSTRUCTION) {
            guard.turnRight();
        }    
        else {
            guard.row = next.first;
            guard.col = next.second;
            patrolRoute.insert({guard.row, guard.col});
        }
    }

    patrolRoute.erase({initialGuardState.row, initialGuardState.col});

    int infiniteLoopCount = 0;
    
    // Allocate the state tracker ONCE
    int rows = map.size();
    int cols = map[0].size();
    std::vector<char> stateVisited(rows * cols * 4, 0);

    for (const auto& [r,c] : patrolRoute) {

        map[r][c] = OBSTRUCTION;

        // Quickly wipe the state tracker clean for this run
        std::fill(stateVisited.begin(), stateVisited.end(), 0);

        if (simulateLoop(stateVisited)) {
            infiniteLoopCount++;
        }

        map[r][c] = EMPTY;
    }

    return infiniteLoopCount;
}