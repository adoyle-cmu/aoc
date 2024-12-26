#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

/**
 * Reads the input file into a matrix (nested int vectors), where '.' is represented as a 0
 * and '#' is represented as a 1.
 * 
 * As it parses the input file, if it encounters a '^' character, it will represent this as a 0 
 * in the matrix and store the coordinates and direction in a 2x2 matrix: [[xpos, ypos], [xdir, ydir]].
 * Since we always start facing north, the direction vector will be [0, -1].
 * 
 * @param filename The name of the file to read.
 * @param input A reference to the vector to store the input.
 * @param pos A reference to the vector to store the position and direction.
 */
bool readFileIntoMatrix(const std::string& filename, std::vector<std::vector<int>>& input, std::vector<std::vector<int>>& pos) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }
    std::string line;
    int xpos = 0;
    int ypos = 0;
    int xdir = 0;
    int ydir = -1;
    while (std::getline(file, line)) {
        std::vector<int> row;
        for (char c : line) {
            switch (c) {
                case '.':
                    row.push_back(0);
                    break;
                case '#':
                    row.push_back(1);
                    break;
                case '^':
                    row.push_back(0);
                    pos.push_back({xpos, ypos});
                    pos.push_back({xdir, ydir});
                    break;
                default:
                    break;
            }
            xpos++;
        }
        input.push_back(row);
        xpos = 0;
        ypos++;
    }
    // Close the file
    file.close();
    return true;
}

/**
 * Helper to determine the next position to move to.
 * The rules are simple: continue in the current direction if the next position is a 0.
 * Otherwise, turn right 90 degrees
 * 
 * If we move, we need to increment the number of positions we've traversed.
 * 
 * If we exit the bounds of the matrix, we need to set the complete flag to true.
 * 
 * @param pos The current position and direction.
 * @param input The input matrix.
 * @param count The number of positions traversed.
 * @param complete Flag to indicate if we've exited the bounds of the matrix.
 */
void move(std::vector<std::vector<int>>& pos, std::vector<std::vector<int>>& input, int& count, bool& complete) {
    // Get the current position and direction
    int xpos = pos[0][0];
    int ypos = pos[0][1];
    int xdir = pos[1][0];
    int ydir = pos[1][1];
    // get the bounds of the matrix
    int rows = input.size();
    int cols = input[0].size();
    // first check if we're about to leave the bounds of the matrix
    if (xpos + xdir < 0 || xpos + xdir >= cols || ypos + ydir < 0 || ypos + ydir >= rows) {
        complete = true;
    }
    // Check if the next position is a 0
    else if (input[ypos + ydir][xpos + xdir] != 1) {
        if (input[ypos][xpos] != 2) {
            // update the input matrix to a 2 to show that we've visited this position
            input[ypos][xpos] = 2;
            count++;
        }
        // update the position
        pos[0][0] += xdir;
        pos[0][1] += ydir;

    } else {
        // Turn right 90 degrees
        if (xdir == 0) {
            pos[1][0] = -ydir;
            pos[1][1] = 0;
        } else {
            pos[1][0] = 0;
            pos[1][1] = xdir;
        }
    }
}

/**
 * Helper to conduct the traversal of the matrix.
 * We will move until the complete flag is set to true.
 * 
 * @param pos The starting position and direction.
 * @param input The input matrix.
 * @param count The number of positions traversed.
 * @param complete Flag to indicate if we've exited the bounds of the matrix.
 */
void traverse(std::vector<std::vector<int>>& pos, std::vector<std::vector<int>>& input, int& count, bool& complete) {
    while (!complete) {
        move(pos, input, count, complete);
    }
}

/**
 * Main block. Receives a filename as an argument and reads the file into a matrix and a vector.
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

    // Matrix to store the input
    std::vector<std::vector<int>> input;

    // Vector to store the position and direction
    std::vector<std::vector<int>> pos;

    // Int to store the number of positions traversed
    int count = 1;

    // Flag to indicate if we've exited the bounds of the matrix
    bool complete = false;

    // Read the file into input and pos
    if (!readFileIntoMatrix(filename, input, pos)) {
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
        std::cout << "Position and Direction:" << std::endl;
        for (const std::vector<int>& p : pos) {
            for (int val : p) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }
    // Traverse the matrix
    traverse(pos, input, count, complete);
    // Output the number of positions traversed
    std::cout << "Number of positions traversed: " << count << std::endl;

    return 0;
}