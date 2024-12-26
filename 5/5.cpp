#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

/**
 * Reads the input file in the following format:
 * for each num1|num2 pair, start a map of int: vector<int>, where the key is num1 and num2 is stored in the vector.
 * Once a blank line is encountered, store each line as a nested vector of integers.
 * 
 * @param filename The name of the file to read.
 * @param orderRules A nested list of pairs of integers.
 * @param input A reference to the vector to store the input.
 */
bool readFileIntoMap(const std::string& filename, std::vector<std::vector<int>>& orderRules, std::vector<std::vector<int>>& input) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }

    // Read the file line by line
    std::string line;
    while (std::getline(file, line)) {
        // Check if the line is empty
        if (line.empty()) {
            break;
        }
        // Create a string stream from the line
        std::istringstream iss(line);
        // Read the num1 and num2 pair
        int num1, num2;
        char sep;
        iss >> num1 >> sep >> num2;
        // Store the pair in the map
            orderRules.push_back({num1, num2});
        }

    // Read the rest of the file into a nested vector
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::vector<int> row;
        std::string item;
        while (std::getline(iss, item, ',')) {
            row.push_back(std::stoi(item));
        }
        input.push_back(row);
    }

    // Close the file
    file.close();

    return true;
}

/** 
 * Helper to determine if a single input row follows the ordering rules
 * 
 * @param row The row to check.
 * @param orderRules The map containing the order of the pairs.
 */
bool followsOrder(const std::vector<int>& row, const std::vector<std::vector<int>>& orderRules) {
    // iterate through the rules
    for (const std::vector<int>& rule : orderRules) {
        // check if both ints in the pair are in the row
        if (!(std::find(row.begin(), row.end(), rule[0]) == row.end() || std::find(row.begin(), row.end(), rule[1]) == row.end())) {
            // check that rule[0] is to the left of rule[1]
            if (std::find(row.begin(), row.end(), rule[0]) > std::find(row.begin(), row.end(), rule[1])) {
                return false;
            }
        }
    }

return true;
}
/**
 * Helper to correct the misordered rows
 * 
 * @param row The row to check.
 * @param orderRules The map containing the order of the pairs.
 */
void correctOrder(std::vector<int>& row, const std::vector<std::vector<int>>& orderRules) {
    while (!followsOrder(row, orderRules)){
        // iterate through the rules
        for (const std::vector<int>& rule : orderRules) {
            // check if both ints in the pair are in the row
            if (!(std::find(row.begin(), row.end(), rule[0]) == row.end() || std::find(row.begin(), row.end(), rule[1]) == row.end())) {
                // check that rule[0] is to the left of rule[1]
                if (std::find(row.begin(), row.end(), rule[0]) > std::find(row.begin(), row.end(), rule[1])) {
                    std::iter_swap(std::find(row.begin(), row.end(), rule[0]), std::find(row.begin(), row.end(), rule[1]));
                }
            }
        }
    }
}

/**
 * Check each row in the input to see if it follows the ordering rules. 
 * Sum the middle values of the rows that follow the rules.
 * 
 * @param input The input vector to check.
 * @param orderRules The map containing the order of the pairs.
 * @param total The sum of the middle values of the rows that follow the rules.
 */
void sumMiddleValues(std::vector<std::vector<int>>& input, 
    const std::vector<std::vector<int>>& orderRules,
    int& total,
    int& correctedTotal) {
    // iterate through the input
    for (std::vector<int>& row : input) {
        // check if the row follows the order
        if (followsOrder(row, orderRules)) {
            total += row[row.size() / 2];
        } else {
            correctOrder(row, orderRules);
            correctedTotal += row[row.size() / 2];
        }
    }
}

/** 
 * Main block. Receives a filename as an argument and reads the file into a map and a nested vector.
 */
int main(int argc, char* argv[]) {
    // Check if the file name was provided as an argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Get the file name from the command-line argument
    std::string filename = argv[1];

    // Map to store the order of the pairs
    std::vector<std::vector<int>> orderRules;

    // Vector to store the input
    std::vector<std::vector<int>> input;

    // Int to store the sum of the middle values
    int total = 0;

    //in to store the corrected total
    int correctedTotal = 0;

    // Read the file into orderRules and input
    if (!readFileIntoMap(filename, orderRules, input)) {
        return 1;
    }
    // check if 'test' is in the filename
    if (filename.find("test") != std::string::npos) {
        // Output the orderRules
        std::cout << "Order Map:" << std::endl;
        for (const auto& pair : orderRules) {
            std::cout << pair[0] << " | " << pair[1] << std::endl;
        }

        // Output the input
        std::cout << "Input:" << std::endl;
        for (const std::vector<int>& row : input) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << std::endl;
        }
    }

    // Sum the middle values of the rows that follow the rules
    sumMiddleValues(input, orderRules, total, correctedTotal);
    // Output the total
    std::cout << "Total: " << total << std::endl;
    std::cout << "Corrected Total: " << correctedTotal << std::endl;

    return 0;
}