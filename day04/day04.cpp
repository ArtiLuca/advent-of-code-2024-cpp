#include "day04.h"

#include <fstream>
#include <stdexcept>


// =============== PUZZLE INPUT & CONSTANTS ===============

const int WordSearch::dr[] = {1, 1, 1, 0, 0, -1, -1, -1};
const int WordSearch::dc[] = {-1, 0, 1, -1, 1, -1, 0, 1};

const std::string WordSearch::target = "XMAS";

void WordSearch::readPuzzleInput() {

    // clear previous 
    grid.clear();

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // we use the first line to dynamically determine padding per line 
    std::string line;
    bool isFirstLine = true;
    std::string edgePadding = "";
    const int paddingLayers = 3;

    // parse input line by line to create padded grid
    while (std::getline(file, line)) {

        // skip empty lines
        if (line.empty())
            continue;

        // handle top boundary padding dynamically based on the first valid line's width
        if (isFirstLine) {
            int paddedLength = line.length() + (paddingLayers * 2);
            edgePadding = std::string(paddedLength, '.');

            // push top padding layers
            for (int i = 0; i < paddingLayers; ++i) 
                grid.push_back(edgePadding);

            isFirstLine = false;        
        }

        // wrap original line with three '.' on both left and right sides
        std::string paddedLine = std::string(paddingLayers, '.') + line + std::string(paddingLayers, '.');
        grid.push_back(paddedLine);
    }

    // push bottom padding layers
    for (int i = 0; i < paddingLayers; ++i)
        grid.push_back(edgePadding);

    file.close();    
}

// =============== PART 1 ===============

int WordSearch::solvePart1() const {

    // guard
    if (grid.empty())
        return 0;

    int rows = grid.size();
    int cols = grid[0].length();    
    int sum = 0;
    
    // iterate entire effective grid
    for (int r = 3; r < rows - 3; ++r) {
        for (int c = 3; c < cols - 3; ++c) {

            // if we encounter an 'X'
            if (grid[r][c] == target[0]) {

                // check 8 directions and 3 subsequent characters
                for (int d = 0; d < 8; ++d) {
                    bool found = true;

                    // no risk of out-of-bounds since we applied padding
                    for (int i = 1; i < 4; ++i) {
                        if (grid[r + i * dr[d]][c + i * dc[d]] != target[i]) {
                            found = false;
                            break;
                        }
                    }
                    if (found)
                        sum++;
                }
            }
        }
    }

    return sum;
}

// =============== PART 2 ===============

int WordSearch::solvePart2() const {

    // guard 
    if (grid.empty())
        return 0;

    int total = 0;
    int rows = grid.size();
    int cols = grid[0].length();    

    // iterate entire effective grid (as in Part 1)
    for (int r = 3; r < rows - 3; ++r) {
        for(int c = 3; c < cols - 3; ++c) {

            // if we find a potential X-MAS center
            if (grid[r][c] == 'A') {

                // extract characters from first diagonal
                char topL = grid[r-1][c-1];
                char bottomR = grid[r+1][c+1];

                // extract characters from second diagonal
                char topR = grid[r-1][c+1];
                char bottomL = grid[r+1][c-1];

                // check top-left to bottom-right diagonal (MAS or SAM)
                bool firstDiagonal = ((topL == 'M' && bottomR == 'S') || (topL == 'S' && bottomR == 'M'));
                // check top-right to bottom-left diagonal (MAS or SAM)
                bool secondDiagonal = ((topR == 'M' && bottomL == 'S') || (topR == 'S' && bottomL == 'M'));

                if (firstDiagonal && secondDiagonal)
                    total++;
            }
        }
    }

    return total;
}