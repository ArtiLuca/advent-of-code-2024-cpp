#include "day03.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <cctype>

// =============== PUZZLE INPUT ===============

void Program::readPuzzleInput() {

    // clear previous 
    operations.clear();

    std::ifstream file(puzzleInput);

    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // read entire file into 'memory' as one continuous string
    memory = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    size_t pos = 0;
    while ((pos = memory.find(MUL_OP,pos)) != std::string::npos) {

        Instruction istr;
        if (parseNextInstruction(pos,istr)) 
            operations.push_back(istr);
        
        else 
            pos++;
        
    }

    file.close();    
}

bool Program::parseNextInstruction(size_t& pos, Instruction& istr) {

        size_t current = pos + 4;

        std::string value_x = "";
        while (current < memory.length() && std::isdigit(memory[current])) {
            value_x += memory[current];
            current++;
        }

        if (value_x.empty() || 
            current >= memory.length() || 
            memory[current] != SEPARATING_COMMA
        ) 
            return false;
        

        // skip comma
        current++;

        std::string value_y = "";
        while (current < memory.length() && std::isdigit(memory[current])) {
            value_y += memory[current];
            current++;
        }

        if (value_y.empty() || 
            current >= memory.length() || 
            memory[current] != CLOSING_PARENTHESIS) 
            return false;
        
        // we successfully parsed numbers
        long long x = 0;
        for (char c : value_x)
            x = x * 10 + (c - '0');

        long long y = 0;
        for (char c : value_y)
            y = y * 10 + (c - '0');

        istr.multiplicand = x;
        istr.multiplier = y;        
        pos = current + 1;
        return true;
}

// =============== PART 1 ===============

long long Program::solvePart1() const {

    // guard 
    if (operations.empty())
        return 0;

    long long total = 0;
    for (const Instruction i : operations) 
        total += (i.multiplicand * i.multiplier);

    return total;    
}



// =============== PART 2 ===============

void Program::readPuzzleInputPart2() {

    // clear previous
    operations.clear();
    memory.clear();

    std::ifstream file(puzzleInput);
    if (!file.is_open()) {
        throw std::runtime_error("Error opening file!");
    }

    // read entire file into 'memory' as one continuous string
    memory = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    size_t pos = 0;
    // by default multiplication operations are enabled at the start
    bool state = true;

    while (pos < memory.length()) {

        // search foward for the three operations 
        size_t next_do = memory.find(DO_OP, pos);
        size_t next_dont = memory.find(DONT_OP, pos);
        size_t next_mul = memory.find(MUL_OP, pos);

        // find the closest one
        size_t next = std::min({next_do, next_dont, next_mul});

        // if all three return std::string::npos, we reach EOF, so we are done
        if (next == std::string::npos)  
            break;

        // handle cases, depending on which instruction was found
        if (next == next_do) {
            state = true;
            // move just past "do()""
            pos = next_do + 4;
        }    
        else if (next == next_dont) {
            state = false;
            // move just past "don't()"
            pos = next_dont + 7;
        }
        else if (next == next_mul) {
            Instruction istr;
            // align pos exactly at "mul(" for parsing helper to work as intended
            pos = next_mul;

            // check instruction with helper
            if (parseNextInstruction(pos, istr)) {
                // set if enabled/disabled
                istr.enabled = state;
                operations.push_back(istr);
            }
            else {
                // failed to parse multiplication operation, we search foward
                pos++;
            }
        }
    }

}

long long Program::solvePart2() const {

    if (operations.empty())
        return 0;

    long long total = 0;    
    for (const Instruction i : operations)
        if (i.enabled)
            total += (i.multiplicand * i.multiplier);   

    return total;    
}