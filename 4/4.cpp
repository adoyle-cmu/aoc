#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <regex>

/**
 * 
 * Reads the input file into a nested vector of integers.
 * Uses the following encoding rule: {X -> 0, M -> 1, A -> 2, S -> 3}
 * Each inner vector represents a line in the file.
 * 
 * @param filename The name of the file to read.
 * @param input A reference to the vector to store the input.
 */
bool readFileIntoVector(const std::string& filename, std::vector<std::vector<int>>& input) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }
    // Read the file into a vector
    std::string line;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char c : line) {
            switch (c) {
                case 'X':
                    row.push_back(0);
                    break;
                case 'M':
                    row.push_back(1);
                    break;
                case 'A':
                    row.push_back(2);
                    break;
                case 'S':
                    row.push_back(3);
                    break;
                default:
                    break;
            }
        }
        input.push_back(row);
    }
    // Close the file
    file.close();
    return true;
}

/**
 * Helper to find horizontal matches in the input
 * A match is a row containing the sequence 0123 or 3210
 * 
 * @param input The input vector to search.
 * @return matches the number of matches found.
 */
int findHorizontalMatches(const std::vector<std::vector<int>>& input) {
    int matches = 0;
    for (const std::vector<int>& row : input) {
        for (int i = 0; i < row.size() - 3; i++) {
            if ((row[i] == 0 && row[i + 1] == 1 && row[i + 2] == 2 && row[i + 3] == 3) ||
                (row[i] == 3 && row[i + 1] == 2 && row[i + 2] == 1 && row[i + 3] == 0)) {
                matches++;
            }
        }
    }
    return matches;
}

/** 
 * Helper to find vertical matches in the input
 * A match is a column containing the sequence 0123 or 3210
 * 
 * @param input The input vector to search.
 * @return matches the number of matches found.
 */
int findVerticalMatches(const std::vector<std::vector<int>>& input) {
    int matches = 0;
    for (int i = 0; i < input[0].size(); i++) {
        for (int j = 0; j < input.size() - 3; j++) {
            if ((input[j][i] == 0 && input[j + 1][i] == 1 && input[j + 2][i] == 2 && input[j + 3][i] == 3) ||
                (input[j][i] == 3 && input[j + 1][i] == 2 && input[j + 2][i] == 1 && input[j + 3][i] == 0)) {
                matches++;
            }
        }
    }
    return matches;
}

/**
 * Helper to find diagonal matches in the input
 * A match is a diagonal containing the sequence 0123 or 3210
 * 
 * @param input The input vector to search.
 * @return matches the number of matches found.
 */
int findDiagonalMatches(const std::vector<std::vector<int>>& input) {
    int matches = 0;
    for (int i = 0; i < input.size() - 3; i++) {
        for (int j = 0; j < input[0].size() - 3; j++) {
            if ((input[i][j] == 0 && input[i + 1][j + 1] == 1 && input[i + 2][j + 2] == 2 && input[i + 3][j + 3] == 3) ||
                (input[i][j] == 3 && input[i + 1][j + 1] == 2 && input[i + 2][j + 2] == 1 && input[i + 3][j + 3] == 0)) {
                matches++;
            }
        }
    }
    return matches;
}

/**
 * Helper to find backwards diagonal matches in the input
 * A match is a backwards diagonal containing the sequence 0123 or 3210
 * 
 * @param input The input vector to search.
 * @return matches the number of matches found.
 */
int findBackwardsDiagonalMatches(const std::vector<std::vector<int>>& input) {
    int matches = 0;
    for (int i = 0; i < input.size() - 3; i++) {
        for (int j = 3; j < input[0].size(); j++) {
            if ((input[i][j] == 0 && input[i + 1][j - 1] == 1 && input[i + 2][j - 2] == 2 && input[i + 3][j - 3] == 3) ||
                (input[i][j] == 3 && input[i + 1][j - 1] == 2 && input[i + 2][j - 2] == 1 && input[i + 3][j - 3] == 0)) {
                matches++;
            }
        }
    }
    return matches;
}

/**
 * Find all matches in the input
 * 
 * @param input The input vector to search.
 * @param matches The number of matches found.
 *
 */
void findAllMatches(const std::vector<std::vector<int>>& input, int& matches) {
    int horizontalMatches = findHorizontalMatches(input);
    std::cout << "Horizontal Matches: " << horizontalMatches << std::endl;
    int verticalMatches = findVerticalMatches(input);
    std::cout << "Vertical Matches: " << verticalMatches << std::endl;
    int diagonalMatches = findDiagonalMatches(input);
    std::cout << "Diagonal Matches: " << diagonalMatches << std::endl;
    int backwardsDiagonalMatches = findBackwardsDiagonalMatches(input);
    std::cout << "Backwards Diagonal Matches: " << backwardsDiagonalMatches << std::endl;
    matches = horizontalMatches + verticalMatches + diagonalMatches + backwardsDiagonalMatches;
}

/**
 * Check for 1,2,3 or 3,1,1 arranged diagonally, and verify that the number in the position 
 * 2 vertical positions below the top left is either 1 or 3, and the the number in the position
 * 2 horizontal positions to the right of the top left is the other in the pair.
 * 
 * @param input The input vector to search.
 * @param smatches The number of matches found.
 */
void findSpecialMatches(const std::vector<std::vector<int>>& input, int& matches) {
    int specialMatches = 0;
    for (int i = 0; i < input.size() - 2; i++) {
        for (int j = 0; j < input[0].size() - 2; j++) {
            if ((input[i][j] == 1 && input[i + 1][j + 1] == 2 && input[i + 2][j + 2] == 3) ||
                (input[i][j] == 3 && input[i + 1][j + 1] == 2 && input[i + 2][j + 2] == 1)) {
                if ((input[i + 2][j] == 1 && input[i][j + 2] == 3) ||
                    (input[i + 2][j] == 3 && input[i][j + 2] == 1)) {
                    specialMatches++;
                }
            }
        }
    }
    matches = specialMatches;
}



/**
 * Main block. Receives a filename as an argument and reads the file into a nested vector.
 * 
 */
int main(int argc, char* argv[]) {
    // Check if the file name was provided as an argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Get the file name from the command-line argument
    std::string filename = argv[1];

    // Vector to store the input
    std::vector<std::vector<int>> input;

    // Int to store the number of matches
    int matches;

    // Int to store the number of special matches
    int specialMatches;

    // Read the file into input
    if (!readFileIntoVector(filename, input)) {
        return 1;
    }

    // Output the input if the filename has 'test' in it
    if (filename.find("test") != std::string::npos) {
        std::cout << "Input:" << std::endl;
        for (const std::vector<int>& row : input) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
    // Find all matches in the input
    findAllMatches(input, matches);
    // Output the number of matches
    std::cout << "Matches: " << matches << std::endl;
    // Find special matches in the input
    findSpecialMatches(input, specialMatches);
    // Output the number of special matches
    std::cout << "Special Matches: " << specialMatches << std::endl;
    
    return 0;
}