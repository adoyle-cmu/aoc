#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

/**
 * Reads in the input file and stores the values in each line as a nested vector.
 * The input files is structured as follows:
 * <solution>: <int1>, <int2>, ...
 * Where each line will be stored as a vector with the solution as the first entry and the values as the subsequent entries.
 * 
 * @param filename The name of the file to read.
 * @param input A reference to the nested vector to store the input.
 */
bool readFileIntoVector(const std::string& filename, std::vector<std::vector<long>>& input) {
    // Open the file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << filename << std::endl;
        return false;
    }
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string solution_str;
        std::getline(iss, solution_str, ':');
        long solution = std::stol(solution_str);
        std::string values_str;
        std::getline(iss, values_str);
        
        std::istringstream values_stream(values_str);
        std::vector<long> values;
        values.push_back(solution); // Add the solution as the first entry
        long value;
        while (values_stream >> value) {
            values.push_back(value);
        }
        
        input.push_back(values);
    }
    // Close the file
    file.close();
    return true;
}

/**
 * Helper to determine if the stored integers in the vector can be combined in any 
 * permutation of multiplication or addition to equal the target value.
 * 
 * @param values The values to combine.
 * @param target The target value.
 * @return True if the values can be combined to equal the target, false otherwise.
 */
bool canCombineToTarget(std::vector<long>& values, long target) {
    // first determine the number of operations needed using the number of values
    int num_operations = values.size() - 2; // Exclude the solution itself
    // now determine the number of permutations of operations
    int num_permutations = 1 << num_operations;
    // iterate through all permutations of operations
    for (int i = 0; i < num_permutations; i++) {
        long result = values[1];
        for (int j = 0; j < num_operations; j++) {
            if (i & (1 << j)) {
                result += values[j + 2];
            } else {
                result *= values[j + 2];
            }
        }
        if (result == target) {
            return true;
        }
    }
    return false;
}

/**
 * Helper to determine if the stored integers in the vector can be combined in any
 * permutation of multiplication or addition to or a third operation to equal the target value.
 * 
 * The third operation is the concat operation, which concatenates the two integers into 1 integer.
 * 
 * @param values The values to combine.
 * @param target The target value.
 * @return True if the values can be combined to equal the target, false otherwise.
 */
bool canCombineToTargetWithConcat(std::vector<long>& values, long target) {
    // first determine the number of operations needed using the number of values
    int num_operations = values.size() - 2; // Exclude the solution itself
    // now determine the number of permutations of operations
    int num_permutations = 1 << (2 * num_operations); // 2 bits per operation (00: add, 01: multiply, 10: concat)
    // iterate through all permutations of operations
    for (int i = 0; i < num_permutations; i++) {
        long result = values[1];
        bool valid = true;
        for (int j = 0; j < num_operations; j++) {
            int operation = (i >> (2 * j)) & 3; // Extract 2 bits for the operation
            if (operation == 0) {
                result += values[j + 2];
            } else if (operation == 1) {
                result *= values[j + 2];
            } else if (operation == 2) {
                result = std::stol(std::to_string(result) + std::to_string(values[j + 2]));
            } else {
                valid = false;
                break;
            }
        }
        if (valid && result == target) {
            return true;
        }
    }
    return false;
}

/**
 * Checks each entry in the input vector to see if the values can be combined to equal the target value 
 * using the addition and multiplication operations.
 * 
 * If they can, we add the target value to the running total.
 * 
 * @param input The nested vector of solutions and values.
 * @param total The running total.
 * @param useConcat Whether to use the concat operation.
 */
void checkSolutions(const std::vector<std::vector<long>>& input, long& total, bool useConcat = false) {
    for (const auto& values : input) {
        long solution = values[0];
        if (useConcat) {
            if (canCombineToTargetWithConcat(const_cast<std::vector<long>&>(values), solution)) {
                total += solution;
            }
        } else {
            if (canCombineToTarget(const_cast<std::vector<long>&>(values), solution)) {
                total += solution;
            }
        }
    }
}

/**
 * Main block. Receives a filename as an argument and reads the file into a nested vector.
 */
int main(int argc, char* argv[]) {
    // Check if the file name was provided as an argument
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    // Get the file name from the command-line argument
    std::string filename = argv[1];

    // Nested vector to store the input
    std::vector<std::vector<long>> input;
    // long to store the running total
    long total = 0;

    if (readFileIntoVector(filename, input)) {
        // If the file name has 'test' in it, output the input
        if (filename.find("test") != std::string::npos) {
            std::cout << "Input:" << std::endl;
            for (const auto& values : input) {
                for (long i : values) {
                    std::cout << i << " ";
                }
                std::cout << std::endl;
            }
        }
        // Check the solutions
        checkSolutions(input, total);
        // Emit the total
        std::cout << "Total: " << total << std::endl;
        // Check the solutions with the concat operation
        total = 0;
        checkSolutions(input, total, true);
        // Emit the total
        std::cout << "Total with concat: " << total << std::endl;

    }
    return 0;
}
